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
	char* c = "Hello world";
	
	print_string("GUI Test Start-----------------------------\n");
	
	printf("a = %d, b = %d, c = %s\n", a, b, c);
	printf("a = %x, b = %x, c = %x\n", a, b, c);
	
	print_string("GUI Test End-------------------------------\n");
	
	return status;
}

COMMAND_INSTALL(test, "\tDo Some Test", Test);