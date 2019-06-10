#ifndef _FILE_STRUCT_H_
#define _FILE_STRUCT_H_

#include "GlobalDefine.h"

typedef struct {
  UINT64    Size;
  UINT64    FileSize;
  UINT64    PhysicalSize;
  EFI_TIME  CreateTime;
  EFI_TIME  LastAccessTime;
  EFI_TIME  ModificationTime;
  UINT64    Attribute;
  CHAR16    FileName[1];
} EFI_FILE_INFO;


typedef enum {
  EfiOpenLoadFile,
  EfiOpenMemoryBuffer,
  EfiOpenFirmwareVolume,
  EfiOpenFileSystem,
  EfiOpenBlockIo,
  EfiOpenMaxValue
} EFI_OPEN_FILE_TYPE;

typedef struct {
  UINT8 Type;
  UINT8 SubType;
  UINT8 Length[2];
} EFI_DEVICE_PATH_PROTOCOL;


//typedef UINT32 EFI_STATUS;

typedef VOID* EFI_HANDLE;

typedef VOID EFI_FIRMWARE_VOLUME_PROTOCOL; //ToDo

typedef UINT8 EFI_SECTION_TYPE;

typedef UINT8 EFI_FV_FILETYPE;

typedef UINT32  EFI_FV_FILE_ATTRIBUTES;

typedef UINT64  EFI_PHYSICAL_ADDRESS;

typedef VOID EFI_LOAD_FILE_PROTOCOL; //ToDo

typedef struct {
  UINT64  Size;
  BOOLEAN ReadOnly;
  UINT64  VolumeSize;
  UINT64  FreeSpace;
  UINT32  BlockSize;
  CHAR16  VolumeLabel[1];
} EFI_FILE_SYSTEM_INFO;

typedef struct {
  UINT32  MediaId;
  BOOLEAN RemovableMedia;
  BOOLEAN MediaPresent;

  BOOLEAN LogicalPartition;
  BOOLEAN ReadOnly;
  BOOLEAN WriteCaching;

  UINT32  BlockSize;
  UINT32  IoAlign;

  EFI_LBA LastBlock;
} EFI_BLOCK_IO_MEDIA;

typedef VOID EFI_FILE; //ToDo

typedef struct {
  UINTN                         Version;          // Common information        
  EFI_OPEN_FILE_TYPE            Type;
  EFI_DEVICE_PATH_PROTOCOL      *DevicePath;
  EFI_STATUS                    LastError;
  EFI_HANDLE                    EfiHandle;        
  CHAR8                         *DeviceName;
  CHAR8                         *FileName;

  UINT64                        CurrentPosition; // Information for Seek
  UINT64                        MaxPosition;
 
  UINTN                         Size;             // Valid for all types other than l#:
  UINT8                         *Buffer;          // Information valid for A#:

  EFI_FIRMWARE_VOLUME_PROTOCOL  *Fv;              // Information valid for Fv#: 
  EFI_GUID                      FvNameGuid;       
  EFI_SECTION_TYPE              FvSectionType;    
  EFI_FV_FILETYPE               FvType;           
  EFI_FV_FILE_ATTRIBUTES        FvAttributes;     

  EFI_PHYSICAL_ADDRESS          FvStart;          
  UINTN                         FvSize;           

  EFI_FILE                      *FsFileHandle;    // Information valid for Fs#: or B#:
  EFI_FILE_SYSTEM_INFO          *FsInfo;
  EFI_FILE_INFO                 *FsFileInfo;
  BOOLEAN                       FsFileInfoIsStale;
  EFI_BLOCK_IO_MEDIA            FsBlockIoMedia;   

  EFI_LOAD_FILE_PROTOCOL        *LoadFile;       // Information valid for l#:

} EFI_OPEN_FILE;


#define EFI_FILE_MODE_READ      0x0000000000000001
#define EFI_FILE_MODE_WRITE     0x0000000000000002
#define EFI_FILE_MODE_CREATE    0x8000000000000000
#define EFI_FILE_MODE_TRUNCATE  0x4000000000000000
#define EFI_FILE_MODE_APPEND    0x2000000000000000

EFI_OPEN_FILE *
EfiOpen (
	IN  CONST CHAR8               *PathName,
	IN  CONST UINT64              OpenMode,
	IN  CONST EFI_SECTION_TYPE    SectionType
);

EFI_STATUS
EfiClose (
	IN  EFI_OPEN_FILE     *Stream
);

UINTN 
EfiTell (
	IN  EFI_OPEN_FILE     *Stream,
	OUT EFI_LBA           *CurrentPosition   OPTIONAL
);

EFI_STATUS
EfiSeek (
	IN  EFI_OPEN_FILE     *Stream,
	IN  EFI_LBA           Offset,
	IN  EFI_SEEK_TYPE     SeekType
);

EFI_STATUS
EfiRead (
	IN  EFI_OPEN_FILE     *Stream,
	OUT VOID              *Buffer,
	OUT UINTN             BufferSize
);
  
EFI_STATUS
EfiWrite (
	IN  EFI_OPEN_FILE   *Stream,
	OUT VOID            *Buffer,
	OUT UINTN           *BufferSize
);



#endif