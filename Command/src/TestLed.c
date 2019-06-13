#include "GlobalDefine.h"
#include "Error.h"
#include "Stdio.h"
#include "Common.h"

#include "ModManager.h"
#include "../Protocol/inc/LedProtocol.h"
#include "CommandManager.h"

/***********************************************************************
Function Name:  		TestLed
Input Paramater:		argc, argv
Function Description:	Led function test
************************************************************************/
MINI2440_STATUS TestLed(int argc, char* const* argv)
{
	printf("In function led_test\n");
	MINI2440_STATUS status = MINI2440_SUCCESS;
	int index = 0;
	
	LedModOps* ledOps = NULL;
	
	gLocateProtocol(MOD_LED, (void*)&ledOps);
	
	for(index = 0; index < LED_MAX_NUMBER; index++)
	{
		printf("Logs\n");
		ledOps->LightLed(index);
		delay(500000);
		ledOps->PutOutLed(index);
	}
	
/*	
	for(index = 0; index < LED_MAX_NUMBER; index++)
	{
		printf("Logs\n");
		LightLed(index);
		delay(500000);
		PutOutLed(index);
	}
*/	
	return status;
}

COMMAND_INSTALL(led, "\tLed test", TestLed);
