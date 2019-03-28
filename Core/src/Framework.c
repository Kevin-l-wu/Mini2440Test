#include <stdio.h>
#include "GlobalDefine.h"
#include "GetoptTemp.h"
#include "Malloc.h"
#include "Error.h"
#include "Common.h"
#include "Gpio.h"
#include "Nand.h"
#include "Lcd.h"
#include "Dm9000.h"
#include "Arp.h"
#include "Tftp.h"
#include "Bootm.h"
#include "Button.h"
#include "I2c.h"
#include "Timer.h"
#include "Dma.h"
#include "Framework.h"
#include "ModManager.h"
#include "../Protocol/inc/LedProtocol.h"



extern void reset(void);

MINI2440_STATUS print_diags_ver(int argc, char(*argv)[MAX_COMMAND_LENGTH]);
MINI2440_STATUS print_diags_sn(int argc, char(*argv)[MAX_COMMAND_LENGTH]);
MINI2440_STATUS run_test(int argc, char(*argv)[MAX_COMMAND_LENGTH]);
MINI2440_STATUS print_help_message();

MINI2440_STATUS sys_reset(int argc, char(*argv)[MAX_COMMAND_LENGTH])
{
	MINI2440_STATUS status = MINI2440_SUCCESS;
	
	reset();
	
	return status;
}

/***********************************************************************
Function Name:  		led_test
Input Paramater:		argc, argv
Function Description:	Led function test
************************************************************************/
MINI2440_STATUS led_test(int argc, char(*argv)[MAX_COMMAND_LENGTH])
{
	print_string("In function led_test\n");
	MINI2440_STATUS status = MINI2440_SUCCESS;
	int index = 0;
	
	LedModOps* ledOps = NULL;
	
	gLocateProtocol(MOD_LED, (void**)&ledOps);
	
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


Diags_Commonds gdiags_commonds[] = {
	{"ver", 	"Diags Version", 			print_diags_ver		},
/*	{"sn", 		"Diags Sn", 				print_diags_sn		},
	{"led", 	"Led Test", 				led_test			},
	{"nand", 	NAND_COMMAND_HELP,			NandTest			},
	{"lcd", 	"Lcd Test", 				lcd_test			},
	{"dm9000", 	"DM9000 Test", 				dm9000_test			},
	{"arp", 	"Arp Test", 				arp_test			},
	{"tftp", 	"Tftp Test", 				tftp_test			},
	{"bootm", 	"Boot Linux kernel", 		bootm				},
	{"button", 	"Button Test", 				button_test			},
	{"i2c", 	"I2c Test", 				I2cTest				},
	{"dma", 	"Dma Testing", 				dma_test			},
	{"test", 	"For Testing", 				run_test			},
	{"timer", 	"Timer Testing", 			TimerTest			},
	{"reset", 	"System reset", 			sys_reset			},
*/	{"help", 	"Help Message", 			print_help_message	},
};

/*******************************************************************
 * Function Name: 	 	print_diags_ver
 * Inputs : 			argc, argv
 * Outputs : 			Status
 * Description:			Print diags ver
 * Author:				Kevin
 *******************************************************************/
MINI2440_STATUS print_diags_ver(int argc, char(*argv)[MAX_COMMAND_LENGTH])
{
	MINI2440_STATUS status = MINI2440_SUCCESS;
	
	print_string("Ver: 20180618\n");
	
	return status;
}

/*******************************************************************
 * Function Name: 	 	print_diags_sn
 * Inputs : 			argc, argv
 * Outputs : 			Status
 * Description:			Print diags sn
 * Author:				Kevin
 *******************************************************************/
MINI2440_STATUS print_diags_sn(int argc, char(*argv)[MAX_COMMAND_LENGTH])
{
	MINI2440_STATUS status = MINI2440_SUCCESS;
	
	print_string("SN: C39HGFG00FF\n");
	
	return status;
}

/*******************************************************************
 * Function Name: 	 	run_test
 * Inputs : 			argc, argv
 * Outputs : 			Status
 * Description:			Run test
 * Author:				Kevin
 *******************************************************************/
MINI2440_STATUS run_test(int argc, char(*argv)[MAX_COMMAND_LENGTH])
{
	MINI2440_STATUS status = MINI2440_SUCCESS;
	
	char* ptr =(char*)malloc(10);

	TestLinkList();
	
	free(ptr);
	
	return status;
}

/*******************************************************************
 * Function Name: 	 	print_help_message
 * Inputs : 			argc, argv
 * Outputs : 			Status
 * Description:			Print help message
 * Author:				Kevin
 *******************************************************************/
MINI2440_STATUS print_help_message(int argc, char(*argv)[MAX_COMMAND_LENGTH])
{
	MINI2440_STATUS status = MINI2440_SUCCESS;
	int index = 0;

	if(argc > 2)
	{
		return MINI2440_INVALID_PARAMETER;
	}
	else if(argc == 2)
	{
		for(index = 0; index < sizeof(gdiags_commonds)/sizeof(gdiags_commonds[0]); index++)
		{
			if(strcmp(gdiags_commonds[index].cmmond_string, argv[1]) == 0)
			{
				print_string(gdiags_commonds[index].cmmond_string);
				print_string(":		");
				print_string(gdiags_commonds[index].message);
				print_string("\n");
				break;
			}
		}
		return MINI2440_INVALID_PARAMETER;
	}
	else
	{
		for(index = 0; index < sizeof(gdiags_commonds)/sizeof(gdiags_commonds[0]); index++)
		{
			print_string(gdiags_commonds[index].cmmond_string);
			print_string(":	");
			print_string(gdiags_commonds[index].message);
			print_string("\n");
		}
	}
	
	return status;
}

/*******************************************************************
 * Function Name: 	 	handle_event_commond
 * Inputs : 			argc, argv
 * Outputs : 			Status
 * Description:			As the command receive from terminal, call corresponding function
 * Author:				Kevin
 *******************************************************************/
void handle_event_commond(char* commond, int argc, char(*argv)[MAX_COMMAND_LENGTH])
{
	unsigned index = 0;
	int compare_result = 0;
	
	for(index = 0; index < sizeof(gdiags_commonds)/sizeof(gdiags_commonds[0]); index++)
	{
		compare_result = strcmp(commond, gdiags_commonds[index].cmmond_string);
		if(compare_result == 0)
		{
			gdiags_commonds[index].function(argc, argv);
			break;
		}
	}
}

/*******************************************************************
 * Function Name: 	 	GenerateArgv
 * Inputs : 			argc, argv, receive_string
 * Outputs : 			void
 * Description:			Generate argc/argv
 * Author:				Kevin
 *******************************************************************/
void GenerateArgv(int* argc, char(*argv)[MAX_COMMAND_NUMBER][MAX_COMMAND_LENGTH], char* receive_string)
{
	char* receive_string_temp = receive_string;
	int str_length = strlen(receive_string) + 1;
	char* ptr_head = receive_string;
	int char_count = 0;
	int index = 0;
	
	*argc = 0;

	for(index = 0; index < str_length; index++)
	{
		char_count++;
		if(receive_string_temp[index] == ' ' || receive_string_temp[index] == '\0')
		{
			memcpy((*argv)[*argc], ptr_head, char_count);
			(*argv)[*argc][char_count - 1] = '\0';
			ptr_head = ptr_head + char_count;
			char_count = 0;
			*argc = *argc + 1;
			
			if(receive_string_temp[index] == '\0')
			{
				break;
			}
		}
	}
}