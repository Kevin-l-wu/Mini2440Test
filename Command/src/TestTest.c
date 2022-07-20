#include "GlobalDefine.h"
#include "Error.h"
#include "CoreMalloc.h"
#include "Stdio.h"

#include "CommandManager.h"

#include "ModManager.h"
#include "../Protocol/inc/BeepProtocol.h"

BeepModOps* gBeepOps = NULL;

/*******************************************************************
 * Function Name: 	 	Test
 * Inputs : 			argc, argv
 * Outputs : 			Status
 * Description:			Print diags ver
 * Author:				Kevin
 *******************************************************************/
static MINI2440_STATUS Test(int argc, char* const* argv)
{
	MINI2440_STATUS status = MINI2440_SUCCESS;
	
	int a = 10;
	int b = 20;
	char c = 'a';
	
	LogPrintf("GUI Test Start-----------------------------\n");
	
	LogPrintf("&a = 0x%x\n", &a);
	
	LogPrintf("a = %d\n", a);
	
	LogPrintf("a = %d, b = %d\n", a, b);
	
	LogPrintf("GUI Test End-------------------------------\n");
	
	return status;
}

COMMAND_INSTALL(test, "\tDo Some Test", Test);