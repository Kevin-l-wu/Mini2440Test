#ifndef GLOBALDEFINE_H_
#define GLOBALDEFINE_H_

#define MAX_COMMAND_NUMBER 10
#define MAX_COMMAND_LENGTH 10


//#define EFI_PROTOCOL_CONSUMER(a)	"../Protocol/inc/"##(EFI_STRINGIZEa)##".h"

//#define DEBUG_ON 1

#define U8 unsigned char
#define U16 unsigned short
#define U32 unsigned int

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

typedef int EFI_GUID;
typedef int EFI_STATUS;
#define EFIAPI

#endif
