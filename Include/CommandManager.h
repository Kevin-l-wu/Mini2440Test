#ifndef _COMMAND_MANAGER_H_
#define _COMMAND_MANAGER_H_

#include "GlobalDefine.h"
#include "Error.h"

typedef struct _DiagsCommand
{
	char* commandString;
	char* message;
	MINI2440_STATUS(*function)(int argc, char* const* argv);
} DiagsCommand;

typedef void (*CommandForEachCallback)(DiagsCommand* pDiagsCommand);

#define STRUCT_SECTION_COMMAND __attribute__ ((unused, section (".Command")))

#define COMMAND_INSTALL(command, message, function) DiagsCommand Command##command \
	STRUCT_SECTION_COMMAND = {#command, message, function};

	
void HandleEventCommond(int argc, char* const* argv);

int CommandForEach(CommandForEachCallback callback);
	
#endif