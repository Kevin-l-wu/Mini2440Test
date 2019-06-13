#ifndef ERROR_H_
#define ERROR_H_

typedef enum _MINI2440_STATUS
{
	MINI2440_SUCCESS,
	MINI2440_INVALID_PARAMETER,
	MINI2440_LOAD_PROTOCOL_FAIL,
} MINI2440_STATUS;

void ErrorCheck(MINI2440_STATUS mini2440Status); 

#define EFI_ERROR_BIT			(0x80000000U)
#define EFI_NOTIFY_BIT			(EFI_ERROR_BIT >> 2)

//
// Set the upper bit to indicate EFI Error.
//
#define EFIERR(a)                 (EFI_ERROR_BIT | (a))
#define EFIWARN(a)                (a)
#define EFINOTIFY(a)              ( EFI_NOTIFY_BIT | (a) )

#define EFI_ERROR(a)              (((INT32) (a)) < 0)
#define EFI_NOTIFY(a)             ( EFI_NOTIFY_BIT & (a) )

#define EFI_STATUS_TO_INT(a)      (int)((INT32)(a))

#define EFI_SUCCESS               0
#define EFI_LOAD_ERROR            EFIERR (1)
#define EFI_INVALID_PARAMETER     EFIERR (2)
#define EFI_UNSUPPORTED           EFIERR (3)
#define EFI_BAD_BUFFER_SIZE       EFIERR (4)
#define EFI_BUFFER_TOO_SMALL      EFIERR (5)
#define EFI_NOT_READY             EFIERR (6)
#define EFI_DEVICE_ERROR          EFIERR (7)
#define EFI_WRITE_PROTECTED       EFIERR (8)
#define EFI_OUT_OF_RESOURCES      EFIERR (9)
#define EFI_VOLUME_CORRUPTED      EFIERR (10)
#define EFI_VOLUME_FULL           EFIERR (11)
#define EFI_NO_MEDIA              EFIERR (12)
#define EFI_MEDIA_CHANGED         EFIERR (13)
#define EFI_NOT_FOUND             EFIERR (14)
#define EFI_ACCESS_DENIED         EFIERR (15)
#define EFI_NO_RESPONSE           EFIERR (16)
#define EFI_NO_MAPPING            EFIERR (17)
#define EFI_TIMEOUT               EFIERR (18)
#define EFI_NOT_STARTED           EFIERR (19)
#define EFI_ALREADY_STARTED       EFIERR (20)
#define EFI_ABORTED               EFIERR (21)
#define EFI_ICMP_ERROR            EFIERR (22)
#define EFI_TFTP_ERROR            EFIERR (23)
#define EFI_PROTOCOL_ERROR        EFIERR (24)
#define EFI_INCOMPATIBLE_VERSION  EFIERR (25)
#define EFI_SECURITY_VIOLATION    EFIERR (26)
#define EFI_CRC_ERROR             EFIERR (27)
#define EFI_END_OF_FILE           EFIERR (31)

#define EFI_WARN_UNKNOWN_GLYPH    EFIWARN (1)
#define EFI_WARN_DELETE_FAILURE   EFIWARN (2)
#define EFI_WARN_WRITE_FAILURE    EFIWARN (3)
#define EFI_WARN_BUFFER_TOO_SMALL EFIWARN (4)
#define EFI_WARN_NO_HW_FEATURE    EFIWARN (5)

#define EFI_NOTIFY_NEED_MORE_ACTION EFINOTIFY(1)

typedef int EFI_STATUS;

#endif