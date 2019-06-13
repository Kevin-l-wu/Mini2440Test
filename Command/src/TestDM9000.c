#include "GlobalDefine.h"
#include "Error.h"
#include "Common.h"
#include "Stdio.h"

#include "ModManager.h"
#include "../Protocol/inc/DM9000Protocol.h"
#include "CommandManager.h"

DM9000ModOps* gDM9000ModOps = NULL;

/*******************************************************************
 * Function Name: 	 	TestDM9000
 * Inputs : 			argc, argv
 * Outputs : 			Status
 * Description:			Print diags ver
 * Author:				Kevin
 *******************************************************************/
MINI2440_STATUS TestDM9000(int argc, char* const* argv)
{
	MINI2440_STATUS status = MINI2440_SUCCESS;
	
	gLocateProtocol(MOD_DM9000, (void*)&gDM9000ModOps);
	
	if(gDM9000ModOps != NULL)
	{
		printf("Test DM9000\n");
		
		printf("DM9000 init\n");
		
		gDM9000ModOps->DM9000Init();
	
		delay(1024 * 1024);
		
		printf("DM9000 deinit\n");
	
		gDM9000ModOps->DM9000DeInit();
		
		delay(1024 * 1024);
		
		printf("DM9000 init second\n");
				
		gDM9000ModOps->DM9000Init();
		
		delay(1024 * 1024);
		
		printf("DM9000 deinit second\n");
	
		gDM9000ModOps->DM9000DeInit();
		
		delay(1024 * 1024);
	}
	
	return status;
}

COMMAND_INSTALL(dm9000, "\tTest DM9000", TestDM9000);