#include <stdio.h>
#include "GlobalDefine.h"
#include "Error.h"

#include "CommandManager.h"

/*******************************************************************
 * Function Name: 	 	TestVer
 * Inputs : 			argc, argv
 * Outputs : 			Status
 * Description:			Print diags ver
 * Author:				Kevin
 *******************************************************************/
MINI2440_STATUS TestVer(int argc, char(*argv)[MAX_COMMAND_LENGTH])
{
	MINI2440_STATUS status = MINI2440_SUCCESS;
	
	print_string("Ver: 20180618\n");
	
	return status;
}

COMMAND_INSTALL(ver, "Diags Version", TestVer);
