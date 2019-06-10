#ifndef _NOR_PROTOCOL_H_
#define _NOR_PROTOCOL_H_

#include "GlobalDefine.h"

typedef void (*NOR_FLASH_INIT)();
typedef unsigned int (*NOR_FLASH_READ)(unsigned int address, char* data, unsigned int length);
typedef unsigned int (*NOR_FLASH_WRITE)(unsigned int address, char* data, unsigned int length);
typedef void (*NOR_FLASH_CHIP_ERASE)();

typedef struct _NorModOps {
	NOR_FLASH_INIT NorFlashInit;
	NOR_FLASH_READ NorFlashRead;
	NOR_FLASH_WRITE NorFlashWrite;
	NOR_FLASH_CHIP_ERASE NorFlashChipErase;
} NorModOps;

#endif	// _NOR_PROTOCOL_H_

