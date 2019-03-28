#include <stdio.h>
#include "GlobalDefine.h"
#include "Error.h"

#include "ModManager.h"
#include "../Protocol/inc/TimerProtocol.h"
#include "CommandManager.h"


/*******************************************************************
 * Function Name: 	 	TestReset
 * Inputs : 			argc, argv
 * Outputs : 			Status
 * Description:			System reset
 * Author:				Kevin
 *******************************************************************/
MINI2440_STATUS TestTimer(int argc, char(*argv)[MAX_COMMAND_LENGTH])
{
	int delayTimes = 20;
	MINI2440_STATUS status = MINI2440_SUCCESS;
	
	TimerModOps* pTimerModOps = NULL;
	
	gLocateProtocol(MOD_TIMER, (void*)&pTimerModOps);
	
	if(pTimerModOps != NULL)	
	{
		pTimerModOps->TimerDelaySeconds(TIMER4, 10);
		
		while(!(pTimerModOps->IsTimeout(TIMER4)))
		{
			delay(1000);
			printf_string("Timer4 start: Interrupt reg dump----\n");
		}

		pTimerModOps->TimerStartStop(TIMER4, 0);
		
		pTimerModOps->TimerDelayMs(TIMER4, 5000);
		
		while(!(pTimerModOps->IsTimeout(TIMER4)))
		{
			printf_string("timer4IntCount = %d\n", *(pTimerModOps->pTimer4IntCount));
			printf_string("timer4TimeOut = %d\n", *(pTimerModOps->pTimer4IntCount));
			printf_string("timer4TimeOutFlag = %d\n", *(pTimerModOps->pTimer4IntCount));
		}

		pTimerModOps->TimerStartStop(TIMER4, 0);
		
	}

	return status;
}

COMMAND_INSTALL(timer, "Timer test", TestTimer);