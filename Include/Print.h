#ifndef _PRINT_H_
#define _PRINT_H_

#include "GlobalDefine.h"

#define PREFIX_SIGN         0x02
#define PREFIX_BLANK        0x04
#define LONG_TYPE           0x10
#define OUTPUT_UNICODE      0x40
#define RADIX_HEX           0x80
#define FORMAT_UNICODE      0x100
#define PAD_TO_WIDTH        0x200
#define ARGUMENT_UNICODE    0x400
#define PRECISION           0x800
#define ARGUMENT_REVERSED   0x1000
#define OUTPUT_UNSIGNED     0x2000

//
// Largest number of characters that can be printed out.
//
#define EFI_DRIVER_LIB_MAX_PRINT_BUFFER 2048

//
// Print primitives
//
#define LEFT_JUSTIFY  0x01
#define PREFIX_SIGN   0x02
#define PREFIX_BLANK  0x04
#define COMMA_TYPE    0x08
#define LONG_TYPE     0x10
#define PREFIX_ZERO   0x20
#define UNSIGNED_TYPE 0x40


UINTN
EFIAPI
AsciiVSPrint(
	OUT CHAR8*                          StartOfBuffer,
	IN UINTN                            BufferSize,
	IN CONST CHAR8*                     Format,
	IN VA_LIST                          Marker
);

#endif