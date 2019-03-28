#include <stdio.h>
#include "GlobalDefine.h"
#include "Error.h"

#include "ModManager.h"
#include "CommandManager.h"

extern int Module_begin;
extern int Module_end;


static void PrintHelpCallback(DiagsCommand* pDiagsCommand)
{
	if(pDiagsCommand != NULL)
	{
		printf_string("%s: %s\n", pDiagsCommand->commandString, pDiagsCommand->message);
	}
}

/*******************************************************************
 * Function Name: 	 	TestHelp
 * Inputs : 			argc, argv
 * Outputs : 			Status
 * Description:			Print help message
 * Author:				Kevin
 *******************************************************************/
MINI2440_STATUS TestHelp(int argc, char(*argv)[MAX_COMMAND_LENGTH])
{
	MINI2440_STATUS status = MINI2440_SUCCESS;
	int index = 0;
	
	CommandForEach(PrintHelpCallback);
	
	
	return status;
}

COMMAND_INSTALL(help, "Help Message", TestHelp);
