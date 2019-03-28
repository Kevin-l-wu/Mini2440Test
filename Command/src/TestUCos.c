#include <stdio.h>
#include "GlobalDefine.h"
#include "Error.h"

#include "CommandManager.h"

#include "UCosMain.h"

/*******************************************************************
 * Function Name: 	 	TestUCos
 * Inputs : 			argc, argv
 * Outputs : 			Status
 * Description:			Print diags ver
 * Author:				Kevin
 *******************************************************************/
static MINI2440_STATUS TestUCos(int argc, char(*argv)[MAX_COMMAND_LENGTH])
{
	MINI2440_STATUS status = MINI2440_SUCCESS;
	
	print_string("TestUCos start-----------------------------\n");
	
	UCosMain();
	
	print_string("TestUCos end-------------------------------\n");
	
	return status;
}

COMMAND_INSTALL(ucos, "Do Some TestUCos", TestUCos);