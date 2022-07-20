#include "GlobalDefine.h"
#include "Common.h"
#include "Error.h"
#include "Uart.h"
#include "Stdio.h"
#include "Interrupt.h"
#include "String.h"
#include "Framework.h"
#include "CommandManager.h"

//#define DEBUG_ON 1

extern void light_led();

char gArgvArray[MAX_COMMAND_NUMBER][MAX_COMMAND_LENGTH] = {'\0'};
char* gArgv[MAX_COMMAND_NUMBER];
int gArgc = 0;

int raise(int a)
{

}

void PreGenerateArgv()
{
	int i = 0;
	
	for(i = 0; i < MAX_COMMAND_NUMBER; i++)
	{
		gArgv[i] = gArgvArray[i];
	}
}


int gboot_main()
{
	char recvStr[MAX_COMMAND_LENGTH * MAX_COMMAND_NUMBER] = {'\0'};
	
	Uart0Init(0);
	
//	EnableMasterIrq();
	
//	touch_init();
	
//	dma_init();
	
	LogPrintf("\r\n---------------- Mini2440 Diags ----------------\r\n");
	LogPrintf(":-) ");
	
	while(1)
	{
		memset(recvStr, 0, MAX_COMMAND_LENGTH * MAX_COMMAND_NUMBER); 	// Clear command line buffer
		uart0_recv_string(recvStr);										// Receive command string from terminal
		
		LogPrintf(recvStr);
		LogPrintf("\n");
		
		/* For generating gArgc and gArgv*/
		PreGenerateArgv();
		GenerateArgv(&gArgc, &gArgvArray, recvStr);
		
#ifdef DEBUG_ON
		int index = 0;
		LogPrintf("\ngArgc = %d\n", gArgc);
		
		for(index = 0; index < gArgc; index++)
		{
			LogPrintf("gArgv[%d] = %s\n", index, gArgv[index]);
		}

		int i = 0;
		
		LogPrintf("gArgv = 0x%x\n", gArgv);
		
		for(i = 0; i < 32; i++)
		{
			LogPrintf("0x%x ", *(((char*)gArgv) + i));
		}

		LogPrintf("\n");	
#endif
//		handle_event_commond(recvStr, gArgc, gArgv);
		HandleEventCommond(gArgc, gArgv);
		
		LogPrintf(":-) ");
	}

    return 0;    
}
