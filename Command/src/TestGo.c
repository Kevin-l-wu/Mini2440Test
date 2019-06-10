#include <stdio.h>
#include "GlobalDefine.h"
#include "Error.h"

#include "CommandManager.h"

typedef void(*ADDRESS)();

int do_go_exec(int(*entry)())
{
	return entry();
}

/*******************************************************************
 * Function Name: 	 	TestSn
 * Inputs : 			argc, argv
 * Outputs : 			Status
 * Description:			Print diags sn
 * Author:				Kevin
 *******************************************************************/
MINI2440_STATUS TestGo(int argc, char* const* argv)
{
	MINI2440_STATUS status = MINI2440_SUCCESS;
	int address = 0;
	
	if(argc == 2)
	{
		address = hex_string_to_int(argv[1]);
		
		printf_string("address = 0x%x\n", address);
		
		delay(1024);
		
		do_go_exec((void *)address);
		
//		((ADDRESS)address)();
		
/*		__asm__ __volatile__(
			"mov r0, %[address1]\n"
			"mov pc, r0\n"
			:
			:[address1] "r" (address)
			:"r0"
		);
*/
	}
	
	return status;
}

COMMAND_INSTALL(go, "\tgo address", TestGo);