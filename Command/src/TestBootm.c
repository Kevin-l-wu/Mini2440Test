#include "GlobalDefine.h"
#include "Error.h"
#include "Common.h"
#include "Stdio.h"

#include "CommandManager.h"

extern unsigned int gLoadAddr;

MINI2440_STATUS TestBootm(int argc, char* const* argv)
{
	MINI2440_STATUS status = MINI2440_SUCCESS;
	
	if(argc == 2)
	{
		gLoadAddr = hex_string_to_int(argv[1]);
		LogPrintf("LoadAddr = 0x%x\n", gLoadAddr);
		
		DisableMasterIrq();
		
		boot_linux();
	}
	
	return status;
}

COMMAND_INSTALL(bootm, "\tBoot Kernel", TestBootm);