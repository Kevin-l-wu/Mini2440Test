#include "GlobalDefine.h"
#include "Error.h"
#include "Common.h"
#include "Stdio.h"

#include "ModManager.h"
#include "../Protocol/inc/DM9000Protocol.h"
#include "CommandManager.h"

extern DM9000ModOps* gDM9000ModOps;

MINI2440_STATUS TestArp(int argc, char* const* argv)
{
	MINI2440_STATUS status = MINI2440_SUCCESS;
	
	if(gDM9000ModOps == NULL)
	{
		gLocateProtocol(MOD_DM9000, (void*)&gDM9000ModOps);
	}
	
	if(gDM9000ModOps != NULL)
	{
		if(!(gDM9000ModOps->DM9000CheckInit()))
		{
			gDM9000ModOps->DM9000Init();
		}

		LogPrintf("TestArp start\n");
		
		delay(1024 * 1024);
		
		
		ArpRequest();
		
		delay(1024 * 1024);
			

		gDM9000ModOps->DM9000DeInit();
		
		delay(1024 * 1024);
	}
	
	LogPrintf("TestArp end\n");
	
	return status;
}

COMMAND_INSTALL(arp, "\tTest arp", TestArp);