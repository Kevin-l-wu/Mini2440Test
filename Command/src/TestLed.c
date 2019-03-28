#include <stdio.h>
#include "GlobalDefine.h"
#include "Error.h"

#include "ModManager.h"
#include "../Protocol/inc/LedProtocol.h"
#include "CommandManager.h"

/***********************************************************************
Function Name:  		TestLed
Input Paramater:		argc, argv
Function Description:	Led function test
************************************************************************/
MINI2440_STATUS TestLed(int argc, char(*argv)[MAX_COMMAND_LENGTH])
{
	print_string("In function led_test\n");
	MINI2440_STATUS status = MINI2440_SUCCESS;
	int index = 0;
	
	LedModOps* ledOps = NULL;
	
	gLocateProtocol(MOD_LED, (void*)&ledOps);
	
	for(index = 0; index < LED_MAX_NUMBER; index++)
	{
		print_string("Logs\n");
		ledOps->LightLed(index);
		delay(500000);
		ledOps->PutOutLed(index);
	}
	
/*	
	for(index = 0; index < LED_MAX_NUMBER; index++)
	{
		print_string("Logs\n");
		LightLed(index);
		delay(500000);
		PutOutLed(index);
	}
*/	
	return status;
}

COMMAND_INSTALL(led, "Led test", TestLed);
