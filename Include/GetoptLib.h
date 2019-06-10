#ifndef _GETOPT_LIB_H
#define _GETOPT_LIB_H

#include "GlobalDefine.h"

//
// Data types
//

#define NoArgument          0
#define RequiredArgument    1
#define OptionalArgument    2

//EFI_FORWARD_DECLARATION(GETOPT_OPTION);

//
// Globals
//
extern CHAR8    *OptArg;
extern INTN     OptInd;
extern INTN     OptOpt;
extern INTN     OptErr;
extern INTN     OptReset;

//
// Functions
//
VOID
GetoptReset (
    VOID
    )
;

INTN
GetoptLong (
    IN  INTN           Argc,
    IN  CHAR8           **Argv,
    IN  CHAR8           *OptionString,
	IN  GETOPT_OPTION   *LongOptions,
    OUT INTN           *LongIndex  OPTIONAL
    )
;

INTN
GetoptLongOnly (
    IN  INTN           Argc,
    IN  CHAR8           **Argv,
    IN  CHAR8           *OptionString,
	IN  GETOPT_OPTION   *LongOptions,
    OUT INTN           *LongIndex  OPTIONAL
    )
;

INTN
Getopt (
    IN  INTN           Argc,
    IN  CHAR8           **Argv,
    IN  CHAR8           *OptionString
    )
;

#endif // _GETOPT_LIB_H
