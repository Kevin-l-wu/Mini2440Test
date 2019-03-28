#include <stdio.h>
#include "GlobalDefine.h"
#include "Error.h"

#include "CommandManager.h"

/*******************************************************************
 * Function Name: 	 	TestReset
 * Inputs : 			argc, argv
 * Outputs : 			Status
 * Description:			System reset
 * Author:				Kevin
 *******************************************************************/
MINI2440_STATUS TestReset(int argc, char(*argv)[MAX_COMMAND_LENGTH])
{
	MINI2440_STATUS status = MINI2440_SUCCESS;
	
	reset();
	
	return status;
}

COMMAND_INSTALL(reset, "System reset", TestReset);
