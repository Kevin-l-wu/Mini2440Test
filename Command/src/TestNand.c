#include "GlobalDefine.h"
#include "Error.h"
#include "Stdio.h"
#include "String.h"
#include "Common.h"

#include "GetoptLib.h"

#include "ModManager.h"
#include "../Protocol/inc/NandProtocol.h"
#include "CommandManager.h"

//#define TEST_NAND_DEBUG_ON 1

NandModOps* gNandModOps = NULL;

/*******************************************************************
 * Function Name: 	 	valueDump
 * Inputs : 			char *valuePool, int dumpLength
 * Outputs : 			void
 * Description:			Dump values
 * Author:				Kevin
 *******************************************************************/
static void valueDump(char *valuePool, int dumpLength)
{
	int index = 0;
	int lineNumber = 0;
	
	for(index = 0; index < dumpLength; index++)
	{
		if(index % 8 == 0)
		{
			LogPrintf("\nLine%d :", lineNumber);
			lineNumber++;
		}
		LogPrintf("0x%C ", valuePool[index]);
	}
}

static void AddrConvert(unsigned address, unsigned* pageAddr, unsigned* columnAddr, int isRandomOperation)
{
	unsigned base = BYTES_PER_PAGE;
	
	if(pageAddr != NULL && columnAddr != NULL)
	{
		if(isRandomOperation)
		{
			base = base + SPARE_BYTES_PER_PAGE;
		}
		
		*pageAddr = address / base;
		*columnAddr = address % base;
	}
}

static void NandReadChipId()
{
	LogPrintf("\n\nStart read ID\n\n");
	int index = 0;
	unsigned chipIDBuf[2] = {0};
	
	gNandModOps->NandIdRead(chipIDBuf);
	
	LogPrintf("chipID = ");
	
	for(index = 0; index < 5; index++)
	{
		LogPrintf("0x%C ", ((char*)chipIDBuf)[index]);
	}
	LogPrintf("\n");
	LogPrintf("\n\nRead ID complete\n\n");
}

static void NandBlockEraseTest(int address)
{
	LogPrintf("\n\nNandBlockEraseTest, address(hex) = 0x%x\n\n", address);
	
	unsigned pageAddr = 0;
	unsigned columnAddr = 0;
	
	if((address % 0x20000) != 0)
	{
		LogPrintf("Address Not support\n");
		return ;
	}
	
	AddrConvert(address, &pageAddr, &columnAddr, 0);
	
	gNandModOps->NandBlockErase(pageAddr);
	LogPrintf("\n\nBlock erase complete\n\n");
}

static void NandPageWriteTest(int address, unsigned writeValue)
{
	LogPrintf("\n\nNandPageWriteTest, address(hex) = 0x%x\n\n", address);
	LogPrintf("\n\nProgram value(hex) = 0x%x\n\n", writeValue);
	
	unsigned writeBuff[(BYTES_PER_PAGE / 4)] = {0};
	unsigned pageAddr = 0;
	unsigned columnAddr = 0;
	int index = 0;
	
	for(index = 0; index < (BYTES_PER_PAGE / 4); index++)
	{
		writeBuff[index] = writeValue;
	}
	AddrConvert(address, &pageAddr, &columnAddr, 0);
	
	gNandModOps->NandPageWrite(pageAddr, writeBuff);
	
	LogPrintf("\n\nPage program complete\n\n");
}

static void NandPageReadTest(int address)
{
	LogPrintf("\n\nNandPageReadTest, address(hex) = %x\n\n", address);
	
	unsigned readBuff[(BYTES_PER_PAGE / 4)] = {0};
	unsigned pageAddr = 0;
	unsigned columnAddr = 0;
	
	
	memset(readBuff, 0, (BYTES_PER_PAGE / 4));
	
	AddrConvert(address, &pageAddr, &columnAddr, 0);
	
	gNandModOps->NandPageRead(pageAddr, readBuff);
	
	LogPrintf("\n\nDump page(hex) %x value:\n\n", pageAddr);
	
	valueDump((char*)readBuff, BYTES_PER_PAGE);
	
	LogPrintf("\n\nPage read complete\n\n");
}

static void NandRandomPageReadTest(int address, int length)
{
	LogPrintf("\n\nNand Random Page Read, address(hex) = 0x%x\n\n", address);
	
	unsigned readBuff[((BYTES_PER_PAGE + SPARE_BYTES_PER_PAGE) / 4)] = {0};
	unsigned pageAddr = 0;
	unsigned columnAddr = 0;

	memset(readBuff, 0, (BYTES_PER_PAGE + SPARE_BYTES_PER_PAGE) / 4);
	
	AddrConvert(address, &pageAddr, &columnAddr, 1);
	gNandModOps->NandRandomPageRead(pageAddr, columnAddr, readBuff, length);
	
	LogPrintf("\n\nDump page(hex) 0x%x value:\n\n", pageAddr);
	
	valueDump((char*)readBuff, length);
	
	LogPrintf("\n\nPage random read complete\n\n");
}

static void NandRandomPageProgramTest(int address, unsigned value)
{
	LogPrintf("\n\nNand Random Page program, address(hex) = 0x%x\n\n", address);

	unsigned pageAddr = 0;
	unsigned columnAddr = 0;

	AddrConvert(address, &pageAddr, &columnAddr, 1);
	gNandModOps->NandRandomPageWrite(pageAddr, columnAddr, &value, 1);
	
	LogPrintf("\n\nPage random program complete\n\n");
}

static void BadBlockCheck()
{
	unsigned blockIndex = 0;
	unsigned pageIndex = 0;
	unsigned invidFlag1 = 0;
	unsigned invidFlag2 = 0;
	unsigned badBlockTable[100] = {0};
	unsigned badBlockIndex = 0;
	
	// Check Initial Invalid Block BLOCK_PER_DEVICE
	for(blockIndex = 0; blockIndex < BLOCK_PER_DEVICE; blockIndex++)
	{
		gNandModOps->NandBlockErase(blockIndex * PAGES_PER_BLOCK);

		gNandModOps->NandRandomPageRead(blockIndex * PAGES_PER_BLOCK, 2048, &invidFlag1, 1);
		
		gNandModOps->NandRandomPageRead(blockIndex * PAGES_PER_BLOCK, 2049, &invidFlag2, 1);
		
		
		LogPrintf("\n\n\nBlock : %x\n\n\n", blockIndex);
		LogPrintf("\ninvidFlag1 = %x\n", invidFlag1);
		LogPrintf("\ninvidFlag2 = %x\n", invidFlag2);
		
		if((invidFlag1 & 0xff) != 0xff || (invidFlag2 & 0xff) != 0xff)
		{
			badBlockTable[badBlockIndex] = blockIndex;
			badBlockIndex++;
		}
		
		if(badBlockIndex >= 100)
		{
			break;
		}
	}
	
	//List init bad block
	LogPrintf("Bad block: ");
	for(blockIndex = 0; blockIndex < badBlockIndex; blockIndex++)
	{
		LogPrintf("0x%x ", badBlockTable[blockIndex]);
	}
	LogPrintf("\n");
}



void CopyNandToRam(int address, char* sdramAddr, int size)
{
	unsigned pageAddr = 0;
	unsigned columnAddr = 0;
	unsigned pageIndex = 0;	
	unsigned copyPages = (size / BYTES_PER_PAGE) + 1;
	
	AddrConvert(address, &pageAddr, &columnAddr, 0);
	
	for(pageIndex = 0; pageIndex < copyPages; pageIndex++)
	{	
		gNandModOps->NandPageRead(pageIndex + pageAddr, (unsigned*)(sdramAddr + (BYTES_PER_PAGE * pageIndex)));
	}
}

void NandReadToMem(int memAddr, int address, unsigned int length)
{
	unsigned pageAddr = 0;
	unsigned columnAddr = 0;
	unsigned pageIndex = 0;	
	unsigned copyPages = (length / BYTES_PER_PAGE) + 1;
	
	unsigned char pageBuff[BYTES_PER_PAGE] = {0};
	
	if((address % BYTES_PER_PAGE) != 0)
	{
		LogPrintf("Not Support nand address\n");
		return;
	}
	
	AddrConvert(address, &pageAddr, &columnAddr, 0);
	
	for(pageIndex = 0; pageIndex < copyPages; pageIndex++)
	{
		gNandModOps->NandPageRead(pageAddr + pageIndex, (unsigned int*)pageBuff);
		
		if(length > BYTES_PER_PAGE)
		{
			memcpy((void*)(memAddr + (BYTES_PER_PAGE * pageIndex)), (const void*)pageBuff, BYTES_PER_PAGE);
		}
		
		if(length <= BYTES_PER_PAGE)
		{
			memcpy((void*)(memAddr + (BYTES_PER_PAGE * pageIndex)), (const void*)pageBuff, length);
			break;
		}
		
		length = length - BYTES_PER_PAGE;
	}
}

void NandWriteFromMem(int memAddr, int address, int length)
{
	unsigned pageAddr = 0;
	unsigned columnAddr = 0;
	unsigned pageIndex = 0;	
	unsigned writePages = (length / BYTES_PER_PAGE) + 1;
	
	unsigned char pageBuff[BYTES_PER_PAGE] = {0};
	
	if((address % BYTES_PER_PAGE) != 0)
	{
		LogPrintf("Not Support nand address\n");
		return;
	}
	
	AddrConvert(address, &pageAddr, &columnAddr, 0);
	
	for(pageIndex = 0; pageIndex < writePages; pageIndex++)
	{
		if(length > BYTES_PER_PAGE)
		{
			memcpy((void*)pageBuff, (const void*)(memAddr + (BYTES_PER_PAGE * pageIndex)), BYTES_PER_PAGE);
			gNandModOps->NandPageWrite(pageAddr + pageIndex, (unsigned int*)pageBuff);
		}
		
		if(length <= BYTES_PER_PAGE)
		{
			memset(pageBuff, 0, BYTES_PER_PAGE);
			memcpy((void*)pageBuff, (const void*)(memAddr + (BYTES_PER_PAGE * pageIndex)), length);
			gNandModOps->NandPageWrite(pageAddr + pageIndex, (unsigned int*)pageBuff);
			break;
		}
		
		length = length - BYTES_PER_PAGE;
	}
}

/*******************************************************************
 * Function Name: 	 	NandTest
 * Inputs : 			argc, argv
 * Outputs : 			Status
 * Description:			Run nand flash test
 * Author:				Kevin
 *******************************************************************/
MINI2440_STATUS TestNand(int argc, char* const* argv)
{
	MINI2440_STATUS status = MINI2440_SUCCESS;
	int option = 0;
	int index = 0;
	
	int address = 0;
	int memAddr = 0;
	int length = 0;
	int value = 0;

	if(gNandModOps == NULL)
	{
		gLocateProtocol(MOD_NAND, (void*)&gNandModOps);
	}
	
	delay(5000);
	
	if(gNandModOps != NULL)
	{
		//NandControllerInit();
		LogPrintf("Nand mod test start\n");
		gNandModOps->NandModInit();
		
		GetoptReset();
		
		while ((option = Getopt(argc, (char**)argv, "d:p:e:r:w:t:a:ic")) != -1)
		{
			switch(option)
			{
				case 'd':
					LogPrintf("OptInd = %d\n", OptInd);
					LogPrintf("OptArg = %s\n", OptArg);
					
					if(argc - OptInd >= 0)
					{
						address = hex_string_to_int(argv[OptInd - 1]);
						NandPageReadTest(address);
					}
					break;
					
				case 'p':
					if(argc - OptInd >= 1)
					{
						address = hex_string_to_int(argv[OptInd - 1]);
						LogPrintf("\n\nwrite address(hex) = 0x%x\n\n", address);
						value = hex_string_to_int(argv[OptInd]);
						LogPrintf("\n\nwrite value(hex) = 0x%x\n\n", value);
						NandPageWriteTest(address, value);
					}
					break;
					
				case 'e':
					address = hex_string_to_int(argv[OptInd - 1]);
					NandBlockEraseTest(address);
					break;
					
				case 'r':
					memAddr = hex_string_to_int(argv[OptInd - 1]);
					address = hex_string_to_int(argv[OptInd]);
					length = hex_string_to_int(argv[OptInd + 1]);
					
					LogPrintf("memAddr = 0x%x\n", memAddr);
					LogPrintf("address = 0x%x\n", address);
					LogPrintf("length = 0x%x\n", length);
					
					NandReadToMem(memAddr, address, length);
					break;
				
				case 'w':
					memAddr = hex_string_to_int(argv[OptInd - 1]);
					address = hex_string_to_int(argv[OptInd]);
					length = hex_string_to_int(argv[OptInd + 1]);
					
					LogPrintf("memAddr = 0x%x\n", memAddr);
					LogPrintf("address = 0x%x\n", address);
					LogPrintf("length = 0x%x\n", length);
					
					NandWriteFromMem(memAddr, address, length);
					break;
					
				case 't':
					if(argc - OptInd >= 0)
					{
						address = hex_string_to_int(argv[OptInd - 1]);
						LogPrintf("Address(hex) = 0x%x\n", address);
						
						length = hex_string_to_int(argv[OptInd]);
						LogPrintf("Length(hex) = 0x%x\n", length);
						
						NandRandomPageReadTest(address, length);
					}
					break;
				
				case 'a':
					if(argc - OptInd >= 1)
					{
						address = hex_string_to_int(argv[OptInd - 1]);		
						LogPrintf("\n\nwrite address(hex) = %x\n\n", address);
						value = hex_string_to_int(argv[OptInd]);
						LogPrintf("\n\nwrite value(hex) = %x\n\n", value);
					
						NandRandomPageProgramTest(address, value);
					}
					break;
					
				case 'i':
					NandReadChipId();
					break;
					
				case 'c':
					BadBlockCheck();
					break;
					
				default:
					LogPrintf("Invalid Parameter\n");
					break;
			}
		}

	}
	LogPrintf("Nand Test end\n");

	return status;
}

#define NAND_COMMAND_HELP "\
\tNand test\n\
	-d : Read a page. nand -d address(hex)\n\
	-p : Write a page. nand -p address(hex) value(hex)\n\
	-e : Erase a block. nand -e address(hex)(address should be 0x00, 0x20000, 0x40000)\n\
	-r : Read data to memory. nand -r memAddr(hex) nandAddr(hex) length\n\
	-w : Write data from memory. nand -w memAddr(hex) nandAddr(hex) length\n\
	-t : Random read. nand -t address(hex)\n\
	-a : Random write. nand -a address(hex) value(hex)\n\
	-i : Read nand ID. nand -i\n\
	-c : Bad block check. nand -c\
	"

COMMAND_INSTALL(nand, NAND_COMMAND_HELP, TestNand);
