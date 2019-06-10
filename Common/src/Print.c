//#include <stdarg.h>
#include "StdArg.h"
#include "Print.h"
#include "Error.h"


#define MAXIMUM_VALUE_CHARACTERS 340
//#define BASE_BUF_SIZE	1024

static CHAR8 mHexStr[] = {
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
	'A', 'B', 'C', 'D', 'E', 'F'
};

struct {
	EFI_STATUS      Status;
	const CHAR8          *String;
} StatusString[] = {
	EFI_SUCCESS, "Success",
	EFI_LOAD_ERROR, "Load Error",
	EFI_INVALID_PARAMETER, "Invalid Parameter",
	EFI_UNSUPPORTED, "Unsupported",
	EFI_BAD_BUFFER_SIZE, "Bad Buffer Size",
	EFI_BUFFER_TOO_SMALL, "Buffer Too Small",
	EFI_NOT_READY, "Not Ready",
	EFI_DEVICE_ERROR, "Device Error",
	EFI_WRITE_PROTECTED, "Write Protected",
	EFI_OUT_OF_RESOURCES, "Out of Resources",
	EFI_VOLUME_CORRUPTED, "Volume Corrupt",
	EFI_VOLUME_FULL, "Volume Full",
	EFI_NO_MEDIA, "No Media",
	EFI_MEDIA_CHANGED, "Media changed",
	EFI_NOT_FOUND, "Not Found",
	EFI_ACCESS_DENIED, "Access Denied",
	EFI_NO_RESPONSE, "No Response",
	EFI_NO_MAPPING, "No mapping",
	EFI_TIMEOUT, "Time out",
	EFI_NOT_STARTED, "Not started",
	EFI_ALREADY_STARTED, "Already started",
	EFI_ABORTED, "Aborted",
	EFI_ICMP_ERROR, "ICMP Error",
	EFI_TFTP_ERROR, "TFTP Error",
	EFI_PROTOCOL_ERROR, "Protocol Error",
	EFI_WARN_UNKNOWN_GLYPH, "Warning Unknown Glyph",
	EFI_WARN_DELETE_FAILURE, "Warning Delete Failure",
	EFI_WARN_WRITE_FAILURE, "Warning Write Failure",
	EFI_WARN_BUFFER_TOO_SMALL, "Warning Buffer Too Small",
};

static 
const CHAR8 *_EfiStatusString(EFI_STATUS Status) {
	UINTN ii = 0;
	
	for(ii = 0; ii < sizeof(StatusString)/sizeof(StatusString[0]);
		ii++) {
		if(Status == StatusString[ii].Status) {
			return StatusString[ii].String;
		}
	}
	
	return NULL;
}

UINT64
DivU64x32 (
	IN UINT64   Dividend,
	IN UINTN    Divisor,
	OUT UINTN   *Remainder OPTIONAL
	)
/*++

Routine Description:

  This routine allows a 64 bit value to be divided with a 32 bit value returns 
  64bit result and the Remainder.

Arguments:

  Dividend  - dividend
  Divisor   - divisor
  Remainder - buffer for remainder
 
Returns:

  Dividend  / Divisor
  Remainder = Dividend mod Divisor

--*/
{
  if (Remainder != NULL) {
    *Remainder = Dividend % Divisor;
  }

  return Dividend / Divisor;
}

static 
UINTN
EFIAPI
BasePrintLibValueToString(
	IN OUT CHAR8* Buffer,
	IN INT64 Value,
	IN UINTN Radix
	)
{
    UINTN                       Digits = 0;
    UINTN                       Remainder;
	
    *Buffer++ = 0;
    do
    {
        Value = ( INT64 ) DivU64x32( ( UINT64 ) Value, ( UINT32 ) Radix, &Remainder );
        *Buffer++ = mHexStr[ Remainder ];
        Digits++;
    }
    while ( Value != 0 );
	
    return( Digits );
}

STATIC
CHAR8*
BasePrintLibFillBuffer(
	IN CHAR8* Buffer,
	IN CHAR8* EndBuffer,
	IN INTN   Length,
	IN UINTN  Character,
	IN INTN   Increment
	)
{
    INTN Index;
	
    for(Index = 0; Index < Length && Buffer < EndBuffer; Index++ )
    {
        Buffer[0] = (CHAR8)Character;
        if (Increment > 1) {
            Buffer[1] = (CHAR8)(Character >> 8);
        }
        Buffer += Increment;
    }
	
    return(Buffer);
}


STATIC
UINTN
BasePrintLibVSPrint(
	OUT CHAR8*                      Buffer,
	IN UINTN                        BufferSize,
	IN UINTN                        Flags,
	IN CONST CHAR8*                 Format,
	IN VA_LIST                      Marker
);

STATIC
UINTN
BasePrintLibSPrint(
	OUT CHAR8*                      StartOfBuffer,
	IN UINTN                        BufferSize,
	IN UINTN                        Flags,
	IN CONST CHAR8*                 Format,
	...
	)
{
    VA_LIST                     Marker;
	UINTN result;
	
    VA_START( Marker, Format );
    result = BasePrintLibVSPrint( StartOfBuffer, BufferSize, Flags, Format, Marker);
	VA_END(Marker);
	
	return result;
}


STATIC
UINTN
BasePrintLibVSPrint(
	OUT CHAR8*                      Buffer,
	IN UINTN                        BufferSize,
	IN UINTN                        Flags,
	IN CONST CHAR8*                 Format,
	IN VA_LIST                      Marker
	)
{
    CHAR8                           ValueBuffer[ MAXIMUM_VALUE_CHARACTERS ];
    CHAR8*                          OriginalBuffer;
    CHAR8*                          EndBuffer;
    CONST CHAR8*                    ArgumentString;
    EFI_GUID*                       TmpGuid;
    EFI_TIME*                       TmpTime;
    VOID*                           TmpPointer;
    INT64                           Value;
    EFI_STATUS                      Status;
    UINTN                           ArgumentMask;
    UINTN                           FormatMask = 0x00FF;
    UINTN                           ArgumentCharacter;
    UINTN                           FormatCharacter;
    UINTN                           Count;
    UINTN                           Width;
    UINTN                           Digits;
    UINTN                           Radix;
    UINTN                           Index;
    UINTN                           Precision;
    UINTN                           Character;
    INTN                            BytesPerOutputCharacter = 1;
    INTN                            BytesPerFormatCharacter = 1;
    INTN                            BytesPerArgumentCharacter;
    CHAR8                           Prefix;
    BOOLEAN                         Comma;
    BOOLEAN                         ZeroPad;
    BOOLEAN                         Done;
	BOOLEAN							MinPrecision = FALSE;
	
    if ( BufferSize == 0 )
    {
        return( 0 );
    }
	
    if ( ( Flags & OUTPUT_UNICODE ) != 0 )
    {
        BytesPerOutputCharacter = 2;
    }
	
    BufferSize--;
    OriginalBuffer = Buffer;
    EndBuffer = Buffer + BufferSize * BytesPerOutputCharacter;
	
    if ( ( Flags & FORMAT_UNICODE ) != 0 )
    {
        BytesPerFormatCharacter = 2;
        FormatMask = 0xFFFF;
    }
	
    FormatCharacter = ( Format[ 0 ] | ( Format[ 1 ] << 8  ) ) & FormatMask;
	
    while ( FormatCharacter != 0 && Buffer < EndBuffer )
    {
        Flags &= ( OUTPUT_UNICODE | FORMAT_UNICODE );
        Width       = 0;
        Precision   = 1;
        Prefix      = 0;
        Comma       = FALSE;
        ZeroPad     = FALSE;
        Count       = 0;
        Digits      = 0;
		
        switch ( FormatCharacter )
        {
            case '%':
                for ( Done = FALSE; !Done; )
                {
                    Format += BytesPerFormatCharacter;
                    FormatCharacter = ( Format[ 0 ] | ( Format[ 1 ] << 8  ) ) & FormatMask;
                    switch ( FormatCharacter )
                    {
                        case '.':
                            Flags |= PRECISION;
                            break;
                        case '-':
                            Flags |= LEFT_JUSTIFY;
                            break;
                        case '+':
                            Flags |= PREFIX_SIGN;
                            break;
                        case ' ':
                            Flags |= PREFIX_BLANK;
                            break;
                        case ',':
                            Flags |= COMMA_TYPE;
                            break;
                        case 'L':
                        case 'l':
                            Flags |= LONG_TYPE;
                            break;
                        case 'n':
							if (sizeof(UINTN) == 8) {
								Flags |= LONG_TYPE;
							}
							break;
                        case '*':
                            if ( ( Flags & PRECISION ) == 0 )
                            {
                                Flags |= PAD_TO_WIDTH;
                                Width = VA_ARG( Marker, UINTN );
                            }
                            else
                            {
                                Precision = VA_ARG( Marker, UINTN );
                            }
                            break;
                        case '0':
                            if ( ( Flags & PRECISION ) == 0 )
                            {
                                Flags |= PREFIX_ZERO;
                            }
                        case '1':
                        case '2':
                        case '3':
                        case '4':
                        case '5':
                        case '6':
                        case '7':
                        case '8':
                        case '9':
                            for ( Count = 0; ( ( FormatCharacter >= '0' ) && ( FormatCharacter <= '9' ) ); )
                            {
                                Count = ( Count * 10 ) + FormatCharacter - '0';
                                Format += BytesPerFormatCharacter;
                                FormatCharacter = ( Format[ 0 ] | ( Format[ 1 ] << 8  ) ) & FormatMask;
                            }
                            Format -= BytesPerFormatCharacter;
                            if ( ( Flags & PRECISION ) == 0 )
                            {
                                Flags |= PAD_TO_WIDTH;
                                Width = Count;
                            }
                            else
                            {
                                Precision = Count;
                            }
                            break;
                        case 0:
                            Format -= BytesPerFormatCharacter;
                            Precision = 0;
                        default:
                            Done = TRUE;
                            break;
                    }
                }
				
                switch ( FormatCharacter )
			{
				case 'p':
					Flags &= ~( PREFIX_BLANK | PREFIX_SIGN | PREFIX_ZERO | LONG_TYPE );
					if ( sizeof( VOID * ) > 4 )
					{
						Flags |= LONG_TYPE;
					}
					// 0x prefix for all %p prints
					Buffer = BasePrintLibFillBuffer(Buffer, EndBuffer, 1, '0', BytesPerOutputCharacter);
					Buffer = BasePrintLibFillBuffer(Buffer, EndBuffer, 1, 'x', BytesPerOutputCharacter);
				case 'X':
					Flags |= PREFIX_ZERO;
				case 'x':
					Flags |= RADIX_HEX;
				case 'u':
					Flags |= OUTPUT_UNSIGNED;
				case 'd':
					
					if ( ( Flags & LONG_TYPE ) == 0 )
					{
						if(Flags & OUTPUT_UNSIGNED) {
							Value = VA_ARG( Marker, unsigned int );
						} else {
							Value = VA_ARG( Marker, int );
						}
					}
					else
					{
						if(Flags & OUTPUT_UNSIGNED) {
							Value = VA_ARG( Marker, UINT64 );
						} else {
							Value = VA_ARG( Marker, INT64 );
						}
					}
					if ( ( Flags & PREFIX_BLANK ) != 0 )
					{
						Prefix = ' ';
					}
					if ( ( Flags & PREFIX_SIGN ) != 0 )
					{
						Prefix = '+';
					}
					if ( ( Flags & COMMA_TYPE ) != 0 )
					{
						Comma = TRUE;
					}
					if ( ( Flags & RADIX_HEX ) == 0 )
					{
						Radix = 10;
						if ( Comma )
						{
							Flags &= ~PREFIX_ZERO;
							Precision = 1;
						}
						if (0 == (Flags & OUTPUT_UNSIGNED) && Value < 0 )
						{
							Flags |= PREFIX_SIGN;
							Prefix = '-';
							Value = -Value;
						}
					}
					else
					{
						Radix = 16;
						Comma = FALSE;
						if ( ( Flags & LONG_TYPE ) == 0 && Value < 0 )
						{
							Value = ( unsigned int ) Value;
						}
					}
					
					Count = BasePrintLibValueToString( ValueBuffer, Value, Radix );
					
					if ( Value == 0 && Precision == 0 )
					{
						Count = 0;
					}
					ArgumentString = ( CHAR8 * ) ValueBuffer + Count;
					Digits = Count % 3;
					if ( Digits != 0 )
					{
						Digits = 3 - Digits;
					}
					if ( Comma && Count != 0 )
					{
						Count += ( ( Count - 1 ) / 3 );
					}
					if ( Prefix != 0 )
					{
						Count++;
						Precision++;
					}
					Flags |= ARGUMENT_REVERSED;
					ZeroPad = TRUE;
					if ( ( Flags & PREFIX_ZERO ) != 0 )
					{
						if ( ( Flags & LEFT_JUSTIFY ) == 0 )
						{
							if ( ( Flags & PAD_TO_WIDTH ) != 0 )
							{
								if ( ( Flags & PRECISION ) == 0 )
								{
									Precision = Width;
								}
							}
						}
					}
					break;
				case 's':
				case 'S':
					Flags |= ARGUMENT_UNICODE;
				case 'a':
					// Auto-determine how many characters we have
					if(!(Flags & PRECISION)) {
						Precision = 0;
					}
					ArgumentString = ( CHAR8 * ) VA_ARG( Marker, CHAR8 * );
					if ( ArgumentString == NULL )
					{
						Flags &= ~ARGUMENT_UNICODE;
						ArgumentString = "<null string>";
					}
					break;
				case 'c':
					Character = VA_ARG( Marker, UINTN ) & 0xFFFF;
					ArgumentString = ( CHAR8 * ) &Character;
					Flags |= ARGUMENT_UNICODE;
					break;
				case 'g':
					TmpGuid = VA_ARG( Marker, EFI_GUID * );
					if ( TmpGuid == NULL )
					{
						ArgumentString = "<null guid>";
					}
					else
					{
						BasePrintLibSPrint(
										   ValueBuffer,
										   MAXIMUM_VALUE_CHARACTERS,
										   0,
										   "%08x-%04x-%04x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x",
										   TmpGuid->Data1,
										   TmpGuid->Data2,
										   TmpGuid->Data3,
										   TmpGuid->Data4[ 0 ],
										   TmpGuid->Data4[ 1 ],
										   TmpGuid->Data4[ 2 ],
										   TmpGuid->Data4[ 3 ],
										   TmpGuid->Data4[ 4 ],
										   TmpGuid->Data4[ 5 ],
										   TmpGuid->Data4[ 6 ],
										   TmpGuid->Data4[ 7 ] );
						ArgumentString = ValueBuffer;
					}
					break;
				case 't':
					TmpTime = VA_ARG( Marker, EFI_TIME * );
					if ( TmpTime == NULL )
					{
						ArgumentString = "<null time>";
					}
					else
					{
						BasePrintLibSPrint(
										   ValueBuffer,
										   MAXIMUM_VALUE_CHARACTERS,
										   0,
										   "%02d/%02d/%04d %02d:%02d",
										   TmpTime->Month,
										   TmpTime->Day,
										   TmpTime->Year,
										   TmpTime->Hour,
										   TmpTime->Minute );
						ArgumentString = ValueBuffer;
					}
					break;
				case 'r':
					Status = VA_ARG( Marker, EFI_STATUS );
					Index = 0;
					ArgumentString = _EfiStatusString(Status);
					if(!ArgumentString) {
						ArgumentString = ValueBuffer;
						BasePrintLibSPrint( ( CHAR8 * ) ValueBuffer, MAXIMUM_VALUE_CHARACTERS, 0, "0x%08lx", (UINT64)Status );
					}
					break;
				case '\n':
					ArgumentString = "\n";
					break;
				//case '%':
				default:
					ArgumentString = ( CHAR8 * ) &FormatCharacter;
					//Flags |= ARGUMENT_UNICODE;
					break;
			}
                break;
            //case '\n':
                //ArgumentString = "\n";
                //break;
            default:
                ArgumentString = ( CHAR8 * ) &FormatCharacter;
                //Flags |= ARGUMENT_UNICODE;
                break;
        }
		
        if ( ( Flags & ARGUMENT_UNICODE ) != 0 )
        {
            ArgumentMask = 0xFFFFF;
            BytesPerArgumentCharacter = 2;
        }
        else
        {
            ArgumentMask = 0x00FF;
            BytesPerArgumentCharacter = 1;
        }
        if ( ( Flags & ARGUMENT_REVERSED ) != 0 )
        {
            BytesPerArgumentCharacter = -BytesPerArgumentCharacter;
        }
        else
        {
            for ( Count = 0; Count < Precision || ( ( Flags & PRECISION ) == 0 ); Count++ )
            {
                ArgumentCharacter = ( ( ArgumentString[ Count * BytesPerArgumentCharacter ] & 0xFF ) | ( ( ArgumentString[ Count * BytesPerArgumentCharacter + 1 ] ) << 8 ) ) & ArgumentMask;
                if ( ArgumentCharacter == 0 )
                {
                    break;
                }
            }
        }
        if ( Precision < Count )
        {
            Precision = Count;
        }
        if ( ( Flags & ( PAD_TO_WIDTH | LEFT_JUSTIFY ) ) == ( PAD_TO_WIDTH ) )
        {
            Buffer = BasePrintLibFillBuffer( Buffer, EndBuffer, Width - Precision, ' ', BytesPerOutputCharacter );
        }
        if ( ZeroPad )
        {
            if ( Prefix != 0 )
            {
                Buffer = BasePrintLibFillBuffer( Buffer, EndBuffer, 1, Prefix, BytesPerOutputCharacter );
            }
            Buffer = BasePrintLibFillBuffer( Buffer, EndBuffer, Precision - Count, '0', BytesPerOutputCharacter );
        }
        else
        {
            Buffer = BasePrintLibFillBuffer( Buffer, EndBuffer, Precision - Count, ' ', BytesPerOutputCharacter );
            if ( Prefix != 0 )
            {
                Buffer = BasePrintLibFillBuffer( Buffer, EndBuffer, 1, Prefix, BytesPerOutputCharacter );
            }
        }
        Index = 0;
        if ( Prefix != 0 )
        {
            Index++;
        }
        while ( Index < Count )
        {
            ArgumentCharacter = ( ( ArgumentString[ 0 ] & 0xFF ) | ( ArgumentString[ 1 ] << 8 ) ) & ArgumentMask;
            Buffer = BasePrintLibFillBuffer( Buffer, EndBuffer, 1, ArgumentCharacter, BytesPerOutputCharacter );
            ArgumentString += BytesPerArgumentCharacter;
            Index++;
            if ( Comma )
            {
                Digits++;
                if ( Digits == 3 )
                {
                    Digits = 0;
                    Index++;
                    if ( Index < Count )
                    {
                        Buffer = BasePrintLibFillBuffer( Buffer, EndBuffer, 1, ',', BytesPerOutputCharacter );
                    }
                }
            }
        }
        if ( ( Flags & ( PAD_TO_WIDTH | LEFT_JUSTIFY ) ) == ( PAD_TO_WIDTH | LEFT_JUSTIFY ) )
        {
            Buffer = BasePrintLibFillBuffer( Buffer, EndBuffer, Width - Precision, ' ', BytesPerOutputCharacter );
        }
        Format += BytesPerFormatCharacter;
        FormatCharacter = ( Format[ 0 ] | ( Format[ 1 ] << 8  ) ) & FormatMask;
    }
    BasePrintLibFillBuffer( Buffer, EndBuffer + BytesPerOutputCharacter, 1, 0, BytesPerOutputCharacter );
	
    return( ( Buffer - OriginalBuffer ) / BytesPerOutputCharacter );
}


UINTN
EFIAPI
AsciiVSPrint(
	OUT CHAR8*                          StartOfBuffer,
	IN UINTN                            BufferSize,
	IN CONST CHAR8*                     Format,
	IN VA_LIST                          Marker
	)
{
    return(BasePrintLibVSPrint(StartOfBuffer, BufferSize, 0, Format, Marker));
}