#include "GlobalDefine.h"
#include "Error.h"
#include "Stdio.h"
#include "Common.h"

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
MINI2440_STATUS TestButton(int argc, char* const* argv)
{
	MINI2440_STATUS status = MINI2440_SUCCESS;
	unsigned wait_times = 100;
	LogPrintf("\n--------------- Button Test ----------------\n");
	
	int button = 0;
	gLocateProtocol(MOD_BUTTON, (void*)&gButtonOps);
	
	if(gButtonOps != NULL)
	{
		gButtonOps-> ButtonInit();
		
		while(!gButtonOps->ButtonHit())
		{
			delay(1024 * 1024);
			button = gButtonOps->ButtonGet();
			
			LogPrintf("button = 0x%x\n", button);
		}
		
		gButtonOps->ButtonFlush();
		
		while(!gButtonOps->ButtonHit())
		{
			delay(1024 * 1024);
			button = gButtonOps->ButtonGet();
			
			LogPrintf("button = 0x%x\n", button);
		}
		
		gButtonOps->ButtonFlush();
		
		while(!gButtonOps->ButtonHit())
		{
			delay(1024 * 1024);
			button = gButtonOps->ButtonGet();
			
			LogPrintf("button = 0x%x\n", button);
		}
			
		gButtonOps->ButtonExit();
	}
	
	LogPrintf("\n--------------- Button Test Over ----------------\n");
	
	return status;
}

COMMAND_INSTALL(button, "\tButton test", TestButton);