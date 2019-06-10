#ifndef _GETOPT_PROTOCOL_H_
#define _GETOPT_PROTOCOL_H_

#include "GlobalDefine.h"

//
// Protocol GUID
//

typedef void (*RESET)(void);

typedef int (*GET_OPT_LONG)(
	IN  INTN           Argc,
	IN  CHAR8           **Argv,
	IN  CHAR8           *OptionString,
	IN  GETOPT_OPTION   *LongOptions,
	OUT INTN            *LongIndex  OPTIONAL
); 

typedef int (*GET_OPT_LONG_ONLY)(
	IN  INTN           Argc,
	IN  CHAR8           **Argv,
	IN  CHAR8           *OptionString,
	IN  GETOPT_OPTION   *LongOptions,
	OUT INTN           *LongIndex  OPTIONAL
); 

typedef int (*GET_OPT)(
	IN  INTN           Argc,           
	IN  CHAR8          **Argv,         
	IN  CHAR8          *OptionString   
);

typedef struct _GetOptModOps
{
	RESET Reset;
	GET_OPT_LONG Long;
	GET_OPT_LONG_ONLY LongOnly;
	GET_OPT Basic;
	char** OptArg;
	int* OptInd;
	int* OptOpt;
	int* OptErr;
	int* OptReset;
} GetOptModOps;


#endif	// _GETOPT_PROTOCOL_H_