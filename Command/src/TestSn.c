#include <stdio.h>
#include "GlobalDefine.h"
#include "Error.h"

#include "CommandManager.h"

/*******************************************************************
 * Function Name: 	 	TestSn
 * Inputs : 			argc, argv
 * Outputs : 			Status
 * Description:			Print diags sn
 * Author:				Kevin
 *******************************************************************/
MINI2440_STATUS TestSn(int argc, char(*argv)[MAX_COMMAND_LENGTH])
{
	MINI2440_STATUS status = MINI2440_SUCCESS;
	
	print_string("SN: C39HGFG00FF\n");
	
	return status;
}

COMMAND_INSTALL(sn, "Diags Sn", TestSn);
