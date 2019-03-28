#include <stdio.h>
#include "GlobalDefine.h"
#include "Error.h"

#include "ModManager.h"
#include "../Protocol/inc/ButtonProtocol.h"
#include "CommandManager.h"
ButtonModOps* gButtonOps = NULL;

/*******************************************************************
 * Function Name: 	 	button_test
 * Inputs : 			argc, argv
 * Outputs : 			Status
 * Description:			Button Test
 * Author:				Kevin
 *******************************************************************/
MINI2440_STATUS TestButton(int argc, char(*argv)[MAX_COMMAND_LENGTH])
{
	MINI2440_STATUS status = MINI2440_SUCCESS;
	unsigned wait_times = 100;
	print_string("\n--------------- Button Test ----------------\n");
	
	int button = 0;
	gLocateProtocol(MOD_BUTTON, (void*)&gButtonOps);
	
	if(gButtonOps != NULL)
	{
		gButtonOps-> ButtonInit();
		
		while(!gButtonOps->ButtonHit())
		{
			delay(1024 * 1024);
			button = gButtonOps->ButtonGet();
			
			printf_string("button = 0x%x\n", button);
		}
		
		gButtonOps->ButtonFlush();
		
		while(!gButtonOps->ButtonHit())
		{
			delay(1024 * 1024);
			button = gButtonOps->ButtonGet();
			
			printf_string("button = 0x%x\n", button);
		}
		
		gButtonOps->ButtonFlush();
		
		while(!gButtonOps->ButtonHit())
		{
			delay(1024 * 1024);
			button = gButtonOps->ButtonGet();
			
			printf_string("button = 0x%x\n", button);
		}
			
		gButtonOps->ButtonExit();
	}
	
	print_string("\n--------------- Button Test Over ----------------\n");
	
	return status;
}

COMMAND_INSTALL(button, "Button test", TestButton);