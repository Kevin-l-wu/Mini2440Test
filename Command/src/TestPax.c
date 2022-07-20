#include "GlobalDefine.h"
#include "Error.h"
#include "Stdio.h"

#include "CommandManager.h"

#include "UCosMain.h"

/*******************************************************************
 * Function Name: 	 	TestPax
 * Inputs : 			argc, argv
 * Outputs : 			Status
 * Description:			Print diags ver
 * Author:				Kevin
 *******************************************************************/
static MINI2440_STATUS TestPax(int argc, char* const* argv)
{
	MINI2440_STATUS status = MINI2440_SUCCESS;
	
	LogPrintf("TestPax start-----------------------------\n");
	
//	PaxMain();
	
	LogPrintf("TestPax end-------------------------------\n");
	
	return status;
}

COMMAND_INSTALL(pax, "\tDo Some TestPax", TestPax);