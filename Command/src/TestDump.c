#include <stdio.h>
#include "GlobalDefine.h"
#include "Error.h"

#include "CommandManager.h"

/*******************************************************************
 * Function Name: 	 	TestVer
 * Inputs : 			argc, argv
 * Outputs : 			Status
 * Description:			Print diags ver
 * Author:				Kevin
 *******************************************************************/
MINI2440_STATUS TestDump(int argc, char* const* argv)
{
	MINI2440_STATUS status = MINI2440_SUCCESS;	
	unsigned char* addr = 0;	
	int len = 0;

	if(argc == 3)
	{
		addr = (unsigned char*)(hex_string_to_int(argv[1]));
		len = hex_string_to_int(argv[2]);
			
		printf_string("addr = 0x%x\n", addr);
		printf_string("len = 0x%x", len);
				
		PrintAscii(addr, len, 8);
	}

	return status;
}

COMMAND_INSTALL(dump, "\tMemory Dump", TestDump);
