#include "SimpleFile.h"

EFI_OPEN_FILE *
EfiOpen (
	IN  CONST CHAR8               *PathName,
	IN  CONST UINT64              OpenMode,
	IN  CONST EFI_SECTION_TYPE    SectionType
)
{
	
}

EFI_STATUS
EfiClose (
	IN  EFI_OPEN_FILE     *Stream
)
{
	
}

UINTN 
EfiTell (
	IN  EFI_OPEN_FILE     *Stream,
	OUT EFI_LBA           *CurrentPosition   OPTIONAL
)
{
	
}

EFI_STATUS
EfiSeek (
	IN  EFI_OPEN_FILE     *Stream,
	IN  EFI_LBA           Offset,
	IN  EFI_SEEK_TYPE     SeekType
)
{
	
}

EFI_STATUS
EfiRead (
	IN  EFI_OPEN_FILE     *Stream,
	OUT VOID              *Buffer,
	OUT UINTN             BufferSize
)
{
	
}
  
EFI_STATUS
EfiWrite (
	IN  EFI_OPEN_FILE   *Stream,
	OUT VOID            *Buffer,
	OUT UINTN           *BufferSize
)
{
	
}