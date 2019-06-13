#ifndef GLOBALDEFINE_H_
#define GLOBALDEFINE_H_

#include "Error.h"

#define MAX_COMMAND_NUMBER 15
#define MAX_COMMAND_LENGTH 15

//#define EFI_PROTOCOL_CONSUMER(a)	"../Protocol/inc/"##(EFI_STRINGIZEa)##".h"

//#define DEBUG_ON 1
//Mathlib
#define NAN         0xfffffffe
#define INFINITY    0xffffffff

//EFI COMPAT
#define EFI_MIN(a, b) (((a) < (b)) ? (a) : (b))
#define EFI_MAX(a, b) (((a) > (b)) ? (a) : (b))

#undef MIN
#define MIN(x,y)	EFI_MIN(x,y)

#undef MAX
#define MAX(x,y)	EFI_MAX(x,y)

#define EFI_BREAKPOINT()  { register volatile UINTN i; do {	i = 1; asm volatile("nop"); } while(1 == i); }
#define EFI_DEADLOOP()    { register volatile UINTN i; do {	i = 1; asm volatile("nop"); } while(1 == i); }

#define EFI_FILE_DIRECTORY  0x0000000000000010

typedef enum {
  EfiSeekStart,
  EfiSeekCurrent,
  EfiSeekEnd,
  EfiSeekMax
} EFI_SEEK_TYPE;

#define count_of(x) (sizeof(x)/sizeof(x[0]))

#define offset_of(type, member) (unsigned long)((void *)&(((type *)0)->member))

#define container_of(ptr, c_type, member) (c_type *)(((char *)(void *)ptr) - offset_of(c_type, member))


#define U8 unsigned char
#define U16 unsigned short
#define U32 unsigned int

typedef unsigned int UINTN;
typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;
typedef unsigned long long UINT64;

typedef int INTN;
typedef char INT8;
typedef short INT16;
typedef int INT32;
typedef long long INT64;

typedef char CHAR8;
typedef short CHAR16;

typedef enum _Boolean
{
	FALSE = 0,
	TRUE = 1,
} BOOLEAN;
typedef UINT64 EFI_LBA;

#define IN
#define OUT
#define OPTIONAL
#define STATIC static
#define CONST const



typedef long long int64_t;
typedef int int32_t;
typedef void VOID;



/* GetOpt Common data struct. We defined here temporarily */
typedef struct _GETOPT_OPTION
{
    CONST CHAR8   *Name;          // name of long option
    INTN    HasArgument;    // one of NoArgument, RequiredArgument, and OptionalArgument: whether option takes an argument
    INTN    *Flag;          // if not NULL, set *Flag to Value when option found
    INTN    Value;          // if Flag not NULL, value to set *Flag to; else return Value
} GETOPT_OPTION;


typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

//typedef int EFI_GUID;
typedef struct {
  UINT32  Data1;
  UINT16  Data2;
  UINT16  Data3;
  UINT8   Data4[8];
} EFI_GUID;

#define EFIAPI

#undef NULL
#define NULL ((void*)0)

typedef unsigned int size_t;


#define countof(x)           count_of(x)

//STDARG
#define _EFI_INT_SIZE_OF(n) ((sizeof (n) + sizeof (UINTN) - 1) &~(sizeof (UINTN) - 1))

typedef CHAR8* VA_LIST;
#define VA_START(ap, v) (ap = (VA_LIST) & (v) + _EFI_INT_SIZE_OF (v))
#define VA_ARG(ap, t)   (*(t *) ((ap += _EFI_INT_SIZE_OF (t)) - _EFI_INT_SIZE_OF (t)))
#define VA_END(ap)      (ap = (VA_LIST) 0)
#define VA_COPY(dest, src) ((void)((dest) = (src)))


//#define va_list VA_LIST
#define EFI_ERROR(a)              (((INT32) (a)) < 0)

typedef struct {
  UINT16  Year;
  UINT8   Month;
  UINT8   Day;
  UINT8   Hour;
  UINT8   Minute;
  UINT8   Second;
  UINT8   Pad1;
  UINT32  Nanosecond;
  INT16   TimeZone;
  UINT8   Daylight;
  UINT8   Pad2;
} EFI_TIME;

#endif
