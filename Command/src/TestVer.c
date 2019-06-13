#include "GlobalDefine.h"
#include "Error.h"
#include "Stdio.h"

#include "CommandManager.h"

/*******************************************************************
 * Function Name: 	 	TestVer
 * Inputs : 			argc, argv
 * Outputs : 			Status
 * Description:			Print diags ver
 * Author:				Kevin
 *******************************************************************/
MINI2440_STATUS TestVer(int argc, char* const* argv)
{
	MINI2440_STATUS status = MINI2440_SUCCESS;
	
	printf("Ver: 20190612\n");
	
	return status;
}

COMMAND_INSTALL(ver, "\tDiags Version", TestVer);
