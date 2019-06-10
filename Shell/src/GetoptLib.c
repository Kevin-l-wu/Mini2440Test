#include "GlobalDefine.h"
#include "Error.h"
#include "Uart.h"

#include "ModManager.h"
#include "../Protocol/inc/GetOptProtocol.h"

#include "GetoptLib.h"

CHAR8    *OptArg;
INTN     OptInd;
INTN     OptOpt;
INTN     OptErr;
INTN     OptReset;

GetOptModOps* gGetOptModOps = NULL;

#define InitGetopt()   \
	if(gGetOptModOps == NULL) { \
		gLocateProtocol(MOD_GETOPT, (void*)&gGetOptModOps);\
	}

VOID
ReadGlobals (
    VOID
    )
{
    OptArg   = *gGetOptModOps->OptArg;
    OptInd   = *gGetOptModOps->OptInd;
    OptOpt   = *gGetOptModOps->OptOpt;
    OptErr   = *gGetOptModOps->OptErr;
    OptReset = *gGetOptModOps->OptReset;
}

VOID
WriteGlobals (
    VOID
    )
{
    *gGetOptModOps->OptArg   = OptArg;
    *gGetOptModOps->OptInd   = OptInd;
    *gGetOptModOps->OptOpt   = OptOpt;
    *gGetOptModOps->OptErr   = OptErr;
    *gGetOptModOps->OptReset = OptReset;
}

VOID
GetoptReset (
    VOID
    )
{
    InitGetopt();

    gGetOptModOps->Reset();

    ReadGlobals();

    return;
}

INTN
GetoptLong (
    IN  INTN           Argc,
    IN  CHAR8           **Argv,
    IN  CHAR8           *OptionString,
	IN  GETOPT_OPTION   *LongOptions,
    OUT INTN           *LongIndex  OPTIONAL
    )
{
    INTN    Result;

    InitGetopt();
    
    WriteGlobals();
	
    Result = gGetOptModOps->Long(Argc, Argv, OptionString, LongOptions, LongIndex);

    ReadGlobals();
    
    return Result;
}

INTN
GetoptLongOnly (
    IN  INTN           Argc,
    IN  CHAR8           **Argv,
    IN  CHAR8           *OptionString,
	IN  GETOPT_OPTION   *LongOptions,
    OUT INTN           *LongIndex  OPTIONAL
    )
{
    INTN    Result;

    InitGetopt();
    
    WriteGlobals();
	
    Result = gGetOptModOps->LongOnly(Argc, Argv, OptionString, LongOptions, LongIndex);

    ReadGlobals();
    
    return Result;
}

INTN
Getopt (
    IN  INTN           Argc,
    IN  CHAR8           **Argv,
    IN  CHAR8           *OptionString
    )
{
    INTN    Result;
    
    InitGetopt();
    
    WriteGlobals();
	
    Result = gGetOptModOps->Basic(Argc, Argv, OptionString);

    ReadGlobals();
    
    return Result;
}
