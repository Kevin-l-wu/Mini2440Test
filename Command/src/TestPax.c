#include <stdio.h>
#include "GlobalDefine.h"
#include "Error.h"

#include "CommandManager.h"

#include "UCosMain.h"

/*******************************************************************
 * Function Name: 	 	TestPax
 * Inputs : 			argc, argv
 * Outputs : 			Status
 * Description:			Print diags ver
 * Author:				Kevin
 *******************************************************************/
static MINI2440_STATUS TestPax(int argc, char(*argv)[MAX_COMMAND_LENGTH])
{
	MINI2440_STATUS status = MINI2440_SUCCESS;
	
	print_string("TestPax start-----------------------------\n");
	
	PaxMain();
	
	print_string("TestPax end-------------------------------\n");
	
	return status;
}

COMMAND_INSTALL(pax, "Do Some TestPax", TestPax);