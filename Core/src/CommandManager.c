#include "GlobalDefine.h"
#include "Error.h"
#include "Stdio.h"
#include "String.h"

#include "CommandManager.h"

extern int Command_begin;
extern int Command_end;

void HandleEventCommond(int argc, char* const* argv)
{
	DiagsCommand* pDiagsCommand = NULL;
	char* commond = NULL;
	int i = 0;
	
	if((argv != NULL) && (argc > 0) )
	{
		int compareResult = 0;
		commond = argv[0]; 
		
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

