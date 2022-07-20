#include "GlobalDefine.h"
#include "Error.h"
#include "Stdio.h"

#include "CommandManager.h"

/*******************************************************************
 * Function Name: 	 	TestSn
 * Inputs : 			argc, argv
 * Outputs : 			Status
 * Description:			Print diags sn
 * Author:				Kevin
 *******************************************************************/
MINI2440_STATUS TestSn(int argc, char* const* argv)
{
	MINI2440_STATUS status = MINI2440_SUCCESS;
	
	LogPrintf("SN: C39HGFG00FF\n");
	
	return status;
}

COMMAND_INSTALL(sn, "\tDiags Sn", TestSn);
