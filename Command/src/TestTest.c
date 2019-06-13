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
	
	printf("GUI Test Start-----------------------------\n");
	
	printf("&a = 0x%x\n", &a);
	
	printf("a = %d\n", a);
	
	printf("a = %d, b = %d\n", a, b);
	
	printf("GUI Test End-------------------------------\n");
	
	return status;
}

COMMAND_INSTALL(test, "\tDo Some Test", Test);