#include "GlobalDefine.h"
#include "Error.h"
#include "Stdio.h"
#include "Common.h"

#include "GetoptLib.h"

#include "CommandManager.h"

#include "ModManager.h"
#include "../Protocol/inc/NorProtocol.h"

NorModOps* gNorModOps = NULL;

/*******************************************************************
 * Function Name: 	 	TestNor
 * Inputs : 			argc, argv
 * Outputs : 			Status
 * Description:			Nor Flash Test
 * Author:				Kevin
 *******************************************************************/
MINI2440_STATUS TestNor(int argc, char* const* argv)
{
	MINI2440_STATUS status = MINI2440_SUCCESS;
	
	int option = 0;
	unsigned int memAddr = 0;
	unsigned int norAddr = 0;
	unsigned int length = 0;
	
	unsigned char readData[128] = {'\0'};
	
	if(gNorModOps == NULL)
	{
		gLocateProtocol(MOD_NOR, (void*)&gNorModOps);
	}
	
	if(gNorModOps != NULL)
	{
		GetoptReset();
		
		while ((option = Getopt(argc, (char**)argv, "ew:r:t")) != -1)
		{
			switch(option)
			{
				case 'e':
					LogPrintf("Erase chip\n");
					gNorModOps->NorFlashChipErase();
					break;
				case 'w':
					memAddr = hex_string_to_int(argv[OptInd - 1]);
					norAddr = hex_string_to_int(argv[OptInd]);
					length = hex_string_to_int(argv[OptInd + 1]);
					
					LogPrintf("memAddr = %x\n", memAddr);
					LogPrintf("norAddr = %x\n", norAddr);
					LogPrintf("length = %x\n", length);
					if((memAddr % 2 != 0) || (norAddr % 2 != 0) || (length % 2 != 0))
					{
						LogPrintf("Not support non aligned address or length\n");
						return -1;
					}
					
					gNorModOps->NorFlashWrite(norAddr, (void*)memAddr, length);
					
					break;

				case 'r':
					memAddr = hex_string_to_int(argv[OptInd - 1]);
					norAddr = hex_string_to_int(argv[OptInd]);
					length = hex_string_to_int(argv[OptInd + 1]);
					
					LogPrintf("memAddr = %x\n", memAddr);
					LogPrintf("norAddr = %x\n", norAddr);
					LogPrintf("length = %x\n", length);
					if((memAddr % 2 != 0) || (norAddr % 2 != 0) || (length % 2 != 0))
					{
						LogPrintf("Not support non aligned address or length\n");
						return -1;
					}
					
					gNorModOps->NorFlashRead(norAddr, (void*)memAddr, length);
					
					break;
					
				case 't':
					gNorModOps->NorFlashRead(0x00, readData, 0x60);
		
					PrintAscii(readData, 0x60, 16);
					break;
				default:
					LogPrintf("Invalid Parameter\n");
					break;
			}	
		}
	}
	
	return status;
}

#define NOR_COMMAND_HELP "\
\tNor test\n\
	-e : Erase a block. nor -e\n\
	-r : Read data to memory. nor -r memAddr(hex) norAddr(hex) length\n\
	-w : Write data from memory. nor -w memAddr(hex) norAddr(hex) length\n\
	-t : Test\n\
	(memAddr/norAddr/length should 2 byte aligned)\n"

COMMAND_INSTALL(nor, NOR_COMMAND_HELP, TestNor);
