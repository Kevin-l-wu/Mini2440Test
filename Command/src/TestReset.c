#include "GlobalDefine.h"
#include "Error.h"
#include "Common.h"
#include "Stdio.h"

#include "CommandManager.h"

extern void reset();

/*******************************************************************
 * Function Name: 	 	TestReset
 * Inputs : 			argc, argv
 * Outputs : 			Status
 * Description:			System reset
 * Author:				Kevin
 *******************************************************************/
MINI2440_STATUS TestReset(int argc, char* const* argv)
{
	MINI2440_STATUS status = MINI2440_SUCCESS;
	
	reset();
	
	return status;
}

COMMAND_INSTALL(reset, "\tSystem reset", TestReset);
