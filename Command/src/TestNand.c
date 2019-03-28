#include <stdio.h>
#include "GlobalDefine.h"
#include "Error.h"
#include "Uart.h"
#include "GetoptTemp.h"

#include "ModManager.h"
#include "../Protocol/inc/NandProtocol.h"
#include "CommandManager.h"



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
			printf_string("\nLine%d :", lineNumber);
			lineNumber++;
		}
		printf_string("0x%C ", valuePool[index]);
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
		printf_string("\n\nAfter convert, pageAddr(hex) = 0x%x\n\n", *pageAddr);
		printf_string("\n\nAfter convert, columnAddr(hex) = 0x%x\n\n", *columnAddr);
	}
}

static void NandReadChipId()
{
	print_string("\n\nStart read ID\n\n");
	int index = 0;
	unsigned chipIDBuf[2] = {0};
	
	gNandModOps->NandIdRead(chipIDBuf);
	
	print_string("chipID = ");
	
	for(index = 0; index < 5; index++)
	{
		printf_string("0x%C ", ((char*)chipIDBuf)[index]);
	}
	print_string("\n");
	print_string("\n\nRead ID complete\n\n");
}

static void NandBlockEraseTest(int address)
{
	printf_string("\n\nNandBlockEraseTest, address(hex) = 0x%x\n\n", address);
	
	unsigned pageAddr = 0;
	unsigned columnAddr = 0;
	
	AddrConvert(address, &pageAddr, &columnAddr, 0);
	
	gNandModOps->NandBlockErase(pageAddr);
	print_string("\n\nBlock erase complete\n\n");
}

static void NandPageWriteTest(int address, unsigned writeValue)
{
	printf_string("\n\nNandPageWriteTest, address(hex) = 0x%x\n\n", address);
	printf_string("\n\nProgram value(hex) = 0x%x\n\n", writeValue);
	
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
	
	print_string("\n\nPage program complete\n\n");
}

static void NandPageReadTest(int address)
{
	printf_string("\n\nNandPageReadTest, address(hex) = %x\n\n", address);
	
	unsigned readBuff[(BYTES_PER_PAGE / 4)] = {0};
	unsigned pageAddr = 0;
	unsigned columnAddr = 0;
	
	
	memset(readBuff, 0, (BYTES_PER_PAGE / 4));
	
	AddrConvert(address, &pageAddr, &columnAddr, 0);
	
	gNandModOps->NandPageRead(pageAddr, readBuff);
	
	printf_string("\n\nDump page(hex) %x value:\n\n", pageAddr);
	
	valueDump((char*)readBuff, BYTES_PER_PAGE);
	
	
	print_string("\n\nPage read complete\n\n");
}


static void NandRandomPageReadTest(int address, int length)
{
	printf_string("\n\nNand Random Page Read, address(hex) = 0x%x\n\n", address);
	
	unsigned readBuff[((BYTES_PER_PAGE + SPARE_BYTES_PER_PAGE) / 4)] = {0};
	unsigned pageAddr = 0;
	unsigned columnAddr = 0;

	memset(readBuff, 0, (BYTES_PER_PAGE + SPARE_BYTES_PER_PAGE) / 4);
	
	AddrConvert(address, &pageAddr, &columnAddr, 1);
	gNandModOps->NandRandomPageRead(pageAddr, columnAddr, readBuff, length);
	
	printf_string("\n\nDump page(hex) 0x%x value:\n\n", pageAddr);
	
	valueDump((char*)readBuff, length);
	
	print_string("\n\nPage random read complete\n\n");
}

static void NandRandomPageProgramTest(int address, unsigned value)
{
	printf_string("\n\nNand Random Page program, address(hex) = 0x%x\n\n", address);

	unsigned pageAddr = 0;
	unsigned columnAddr = 0;

	AddrConvert(address, &pageAddr, &columnAddr, 1);
	gNandModOps->NandRandomPageWrite(pageAddr, columnAddr, &value, 1);
	
	print_string("\n\nPage random program complete\n\n");
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
		
		
		printf_string("\n\n\nBlock : %x\n\n\n", blockIndex);
		printf_string("\ninvidFlag1 = %x\n", invidFlag1);
		printf_string("\ninvidFlag2 = %x\n", invidFlag2);
		
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
	print_string("Bad block: ");
	for(blockIndex = 0; blockIndex < badBlockIndex; blockIndex++)
	{
		printf_string("0x%x ", badBlockTable[blockIndex]);
	}
	print_string("\n");
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


/*******************************************************************
 * Function Name: 	 	NandTest
 * Inputs : 			argc, argv
 * Outputs : 			Status
 * Description:			Run nand flash test
 * Author:				Kevin
 *******************************************************************/
MINI2440_STATUS TestNand(int argc, char (*argv)[MAX_COMMAND_LENGTH])
{
	MINI2440_STATUS status = MINI2440_SUCCESS;
	char option = 0;
	int index = 0;
	
	int address = 0;
	int length = 0;
	int value = 0;
	
	printf_string("\n\nargc = %d\n\n", argc);
		
	for(index = 0; index < argc; index++)
	{
		printf_string("argv[%d] = %s\n", index, argv[index]);
	}
	
	if(gNandModOps == NULL)
	{
		gLocateProtocol(MOD_NAND, (void*)&gNandModOps);
	}
	
	printf_string("\n\gNandModOps = 0x%x\n\n", gNandModOps);
	
	delay(5000);
	
	if(gNandModOps != NULL)
	{
		//NandControllerInit();
		print_string("Nand mod test start\n");
		gNandModOps->NandModInit();
		
		GetoptInit();
		
		while ((option = Getopt(argc, argv, "epritca")) != 0)
		{
			switch(option)
			{
				case 'e':
					address = hex_string_to_int(argv[optInd]);
					NandBlockEraseTest(address);
					break;
				
				case 'p':
					if(argc - optInd >= 1)
					{
						address = hex_string_to_int(argv[optInd]);
						printf_string("\n\nargv[optInd + 1] = %s\n\n", argv[optInd + 1]);
						value = hex_string_to_int(argv[optInd + 1]);
						printf_string("\n\nwrite value(hex) = 0x%x\n\n", value);
						NandPageWriteTest(address, value);
					}
					break;
					
				case 'r':
					if(argc - optInd >= 0)
					{
						address = hex_string_to_int(argv[optInd]);
						NandPageReadTest(address);
					}
					break;
					
				case 'i':
					NandReadChipId();
					break;
					
				case 't':
					if(argc - optInd >= 1)
					{
						address = hex_string_to_int(argv[optInd]);
						printf_string("Address(hex) = 0x%x\n", address);
						
						length = hex_string_to_int(argv[optInd + 1]);
						printf_string("Length(hex) = 0x%x\n", length);
						
						NandRandomPageReadTest(address, length);
					}
					break;
					
				case 'c':
					BadBlockCheck();
					break;

				case 'a':
					if(argc - optInd >= 1)
					{
						address = hex_string_to_int(argv[optInd]);
						
						printf_string("\n\nargv[optInd + 1] = %s\n\n", argv[optInd + 1]);
						value = hex_string_to_int(argv[optInd + 1]);
						printf_string("\n\nwrite value(hex) = %x\n\n", value);
					
						NandRandomPageProgramTest(address, value);
					}
					break;
					
				default:
					printf_string("Invalid Parameter\n");
					break;
			}
		}

	}
	print_string("Nand Test end\n");

	return status;
}

#define NAND_COMMAND_HELP "\
Nand test\n\
	-r : Read a page. nand -r pageAddress(hex)\n\
	-p : Write a page. nand -p pageAddress(hex) value(hex)\n\
	-e : Erase a block. nand -e pageAddress(hex)\n\
	-t : Random read. nand -t address(hex)\n\
	-a : Random write. nand -a address(hex) value(hex)\n\
	-i : Read nand ID. nand -i\n\
	-c : Bad block check. nand -c\
	"

COMMAND_INSTALL(nand, NAND_COMMAND_HELP, TestNand);
