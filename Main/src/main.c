#include <stdio.h>
#include <stdarg.h>
#include "GlobalDefine.h"
#include "Common.h"
#include "Error.h"
#include "Gpio.h"
#include "Uart.h"
#include "Interrupt.h"
#include "String.h"
#include "Framework.h"
#include "CommandManager.h"

//#define DEBUG_ON 1

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
	
	Uart0Init();
	
	EnableMasterIrq();
	
//	touch_init();
	
//	dma_init();
	
	print_string("\r\n---------------- Mini2440 Diags ----------------\r\n");
	print_string(":-) ");

	while(1)
	{
		memset(recvStr, 0, MAX_COMMAND_LENGTH * MAX_COMMAND_NUMBER); 	// Clear command line buffer
		uart0_recv_string(recvStr);										// Receive command string from terminal
		
		print_string(recvStr);
		print_string("\n");
		
		/* For generating gArgc and gArgv*/
		PreGenerateArgv();
		GenerateArgv(&gArgc, &gArgvArray, recvStr);
		
#ifdef DEBUG_ON
		int index = 0;
		printf_string("\ngArgc = %d\n", gArgc);
		
		for(index = 0; index < gArgc; index++)
		{
			printf_string("gArgv[%d] = %s\n", index, gArgv[index]);
		}

		int i = 0;
		
		printf_string("gArgv = 0x%x\n", gArgv);
		
		for(i = 0; i < 32; i++)
		{
			printf_string("0x%x ", *(((char*)gArgv) + i));
		}

		printf_string("\n");	
#endif
//		handle_event_commond(recvStr, gArgc, gArgv);
		HandleEventCommond(gArgc, gArgv);
		
		print_string(":-) ");
	}

    return 0;    
}
