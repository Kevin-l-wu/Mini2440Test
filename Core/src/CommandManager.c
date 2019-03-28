#include <stdio.h>
#include "GlobalDefine.h"
#include "Error.h"

#include "CommandManager.h"


extern int Command_begin;
extern int Command_end;


void HandleEventCommond(char* commond, int argc, char(*argv)[MAX_COMMAND_LENGTH])
{
	DiagsCommand* pDiagsCommand = NULL;
	int compareResult = 0;
	
	for(pDiagsCommand = (DiagsCommand*)&Command_begin; pDiagsCommand < (DiagsCommand*)&Command_end; pDiagsCommand++)
	{	
		compareResult = strcmp(commond, pDiagsCommand->commandString);
		if(compareResult == 0)
		{
			pDiagsCommand->function(argc, argv);
			break;
		}
	}
}

int CommandForEach(CommandForEachCallback callback)
{
	DiagsCommand* pDiagsCommand = NULL;

	if(callback != 0)
	{
		for(pDiagsCommand = (DiagsCommand*)&Command_begin; pDiagsCommand < (DiagsCommand*)&Command_end; pDiagsCommand++)
		{
			callback(pDiagsCommand);
		}
	}
	return 0;
}

