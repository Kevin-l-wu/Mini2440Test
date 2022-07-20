#include "GlobalDefine.h"
#include "Error.h"
#include "Stdio.h"
#include "Common.h"

#include "Tftp.h"

#include "ModManager.h"
#include "../Protocol/inc/DM9000Protocol.h"
#include "CommandManager.h"

extern DM9000ModOps* gDM9000ModOps;
extern u8 gTftpFinishFlag;

extern MINI2440_STATUS TestArp(int argc, char* const* argv);

MINI2440_STATUS TestTftp(int argc, char* const* argv)
{
	MINI2440_STATUS status = MINI2440_SUCCESS;
	u32 index = 0;
	
	unsigned char* loadAddr = NULL;
	
	if(argc == 3)
	{
		TestArp(argc, argv);
		
		loadAddr = (unsigned char*)(hex_string_to_int(argv[1]));
		
		TftpSetLoadAddr(loadAddr);
		TftpSetFileName(argv[2]);
		
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
		
			delay(1024 * 1024);
			
			TftpSendRequest();
			
			delay(100);
			
			while(!gTftpFinishFlag)
			{
				if(!(index++ % 50))
				{
					LogPrintf("#");
				}
				
				gDM9000ModOps->DM9000Recv();
			}
		
	//		LogPrintf("The data received:\n");
	//		TftpPrintData();
			delay(1024 * 1024);
			
			gDM9000ModOps->DM9000DeInit();
			
			delay(1024 * 1024);
		}	
	}
	return status;
}

COMMAND_INSTALL(tftp, "\tTest tftp: tftp address filename", TestTftp);