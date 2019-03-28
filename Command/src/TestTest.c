#include <stdio.h>
#include "GlobalDefine.h"
#include "Error.h"
#include "Malloc.h"

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
static MINI2440_STATUS Test(int argc, char(*argv)[MAX_COMMAND_LENGTH])
{
	MINI2440_STATUS status = MINI2440_SUCCESS;
	
	print_string("Test start-----------------------------\n");
	
	gLocateProtocol(MOD_BEEP, (void*)&gBeepOps);
	
	if(gBeepOps != NULL)
	{
		gBeepOps->BeepInit();
		
		gBeepOps->BeepEnable();
		
		delay(1024 * 1024);
		
		gBeepOps->BeepDisable();
	}
	
	print_string("Test end-------------------------------\n");
	
	return status;
}

COMMAND_INSTALL(test, "Do Some Test", Test);