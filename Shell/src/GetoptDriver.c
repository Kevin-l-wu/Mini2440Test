#include "GlobalDefine.h"
#include "Getopt.h"

#include "ModManager.h"
#include "../Protocol/inc/GetOptProtocol.h"

VOID
Reset (
    VOID
    )
{
    optarg   = NULL;
    optind   = 1;
    optopt   = '\0';
    opterr   = 1;
    optreset = 1;
}

/*
 * These wrappers are here for this bug:
 * <rdar://problem/11997947> GetOpt internally returns "int" but protocol defines return value as INTN
 */
static INTN
GetoptLongWrapper (
			IN  INTN           Argc,
			IN  CHAR8           **Argv,
			IN  CHAR8           *OptionString,
			IN  struct option   *LongOptions,
			OUT INTN            *LongIndex  OPTIONAL
				   ) {
	return getopt_long(Argc, Argv, OptionString,
						 (const struct option *)LongOptions,
						 LongIndex);
}

static INTN
GetoptLongOnlyWrapper (
				IN  INTN           Argc,
				IN  CHAR8           **Argv,
				IN  CHAR8           *OptionString,
				IN  struct option   *LongOptions,
				OUT INTN           *LongIndex  OPTIONAL
					   ) {
	return getopt_long_only(Argc, Argv, OptionString,
							  (const struct option *)LongOptions,
							  LongIndex);
}

static INTN
GetoptWrapper (
		IN  INTN           Argc,           
		IN  CHAR8          **Argv,         
		IN  CHAR8          *OptionString   
			   ) {
	return getopt(Argc, Argv, OptionString);
}

GetOptModOps getOptModOps =
{
    .Reset = Reset,
    .Long = GetoptLongWrapper,
    .LongOnly = GetoptLongOnlyWrapper,
    .Basic = GetoptWrapper,
    .OptArg = &optarg,
    .OptInd = &optind,
    .OptOpt = &optopt,
    .OptErr = &opterr,
    .OptReset = &optreset
};

MODULE_INSTALL(Getopt, MOD_GETOPT, 0, &getOptModOps);
