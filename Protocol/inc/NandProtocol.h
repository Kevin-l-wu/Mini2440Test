#ifndef _NAND_PROTOCOL_H_
#define _NAND_PROTOCOL_H_

#include "GlobalDefine.h"

#define BYTES_PER_PAGE 2048
#define PAGES_PER_BLOCK 64
#define BLOCK_PER_DEVICE 2048

#define SPARE_BYTES_PER_SECTOR 16
#define SPARE_BYTES_PER_PAGE 64


typedef void (*NAND_MOD_INIT)();

typedef void (*NAND_RESET)();

typedef void (*NAND_PAGE_READ)(unsigned pageAddr, unsigned* readData);

typedef void (*NAND_RANDOM_PAGE_READ)(unsigned pageAddr, unsigned columnAddr, 
	unsigned* readData, unsigned length);

typedef int (*NAND_PAGE_WRITE)(unsigned pageAddr, unsigned* writeData);

typedef void (*NAND_RANDOM_PAGE_WRITE)(unsigned pageAddr, unsigned columnAddr, 
	unsigned* writeData, unsigned length);

typedef int (*NAND_BLOCK_ERASE)(unsigned pageAddr);	

typedef int (*NAND_ID_READ)(unsigned* chipIDBuf);

typedef void (*NAND_MOD_CLOSE)();

typedef struct _NandModOps {
	NAND_MOD_INIT NandModInit;
	NAND_RESET NandReset;
	NAND_PAGE_READ NandPageRead;
	NAND_RANDOM_PAGE_READ NandRandomPageRead;
	NAND_PAGE_WRITE NandPageWrite;
	NAND_RANDOM_PAGE_WRITE NandRandomPageWrite;
	NAND_BLOCK_ERASE NandBlockErase;
	NAND_ID_READ NandIdRead;
	NAND_MOD_CLOSE NandModClose;
} NandModOps;

#endif	// _NAND_PROTOCOL_H_

