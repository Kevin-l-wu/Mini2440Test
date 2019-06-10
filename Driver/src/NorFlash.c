#include "NorFlash.h"

#include "ModManager.h"
#include "../Protocol/inc/NorProtocol.h"

#define BaseAddr 0
#define FlashAddr16(Offset) (volatile unsigned short*)((BaseAddr)+(Offset))


static void NorFlashReadWord(unsigned int address, unsigned short* data)
{
	*data = *FlashAddr16(address);	
}

static void NorFlashWriteWord(unsigned int address, unsigned short data)
{
//	printf_string("address = %x\n", address);
//	printf_string("data = %x\n", data);
	*FlashAddr16(address) = data;
}

//When standard Program and Erase, DQ6 is toggle
static void CheckToggle()
{
	unsigned short data = *FlashAddr16(0x00);
	unsigned short newData = 0x00;

	while(1)
	{
		newData = *FlashAddr16(0x00);
		if((data & 0x40) == (newData & 0x40))
		{
			break;
		}
		data = newData;
	}
}

void NorFlashInit()
{
	print_string("Please take the trigger to nor flash\n");
}

unsigned int NorFlashRead(unsigned int address, char* data, unsigned int length)
{
	unsigned int readTimes = length / 2;
	unsigned int i = 0;
	
	for(i = 0; i < readTimes; i++)
	{
		NorFlashReadWord(address + (i * 2), (unsigned short*)(data + (i * 2)));
	}
}

unsigned int NorFlashWrite(unsigned int address, char* data, unsigned int length)
{
	unsigned int writeTimes = length / 2;
	unsigned int i = 0;
	
	for(i = 0; i < writeTimes; i++)
	{
		NorFlashWriteWord((0x5555 << 1), 0xaa);
		NorFlashWriteWord((0x2AAA << 1), 0x55);
		NorFlashWriteWord((0x5555 << 1), 0xa0);
		
		NorFlashWriteWord(address + (i * 2), *((unsigned short*)(data + (i * 2))));
	
		CheckToggle();
	}
	
	return 0;
}

void NorFlashChipErase()
{
	printf_string("NorFlashChipErase\n");
	
	NorFlashWriteWord((0x5555 << 1), 0xaa);
	NorFlashWriteWord((0x2AAA << 1), 0x55);
	NorFlashWriteWord((0x5555 << 1), 0x80);
	
	NorFlashWriteWord((0x5555 << 1), 0xaa);
	NorFlashWriteWord((0x2AAA << 1), 0x55);
	NorFlashWriteWord((0x5555 << 1), 0x10);
	
	CheckToggle();
	
	return;
}


NorModOps norModOps = {
	.NorFlashInit = NorFlashInit,
	.NorFlashRead = NorFlashRead,
	.NorFlashWrite = NorFlashWrite,
	.NorFlashChipErase = NorFlashChipErase,
};

MODULE_INSTALL(Nor, MOD_NOR, 0, &norModOps);




