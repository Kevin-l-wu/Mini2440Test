#ifndef NAND_H_
#define NAND_H_

#define TACLS 3
#define TWRPH0 4
#define TWRPH1 3

#define DEBUG_NAND_FLASH 1

#define NFCONF (*(volatile unsigned long*)0x4E000000) 
#define NFCONT (*(volatile unsigned long*)0x4E000004)		//Nand Control Register

#define NFCMD (*(volatile unsigned long*)0x4E000008)
#define NFADDR (*(volatile unsigned long*)0x4E00000C)
#define NFDATA (*(volatile unsigned long*)0x4E000010)

#define NFSTAT (*(volatile unsigned long*)0x4E000020)		//Nand Status Register

/*
void NandPageRead(unsigned pageAddr, unsigned* readData);

int NandProgramPage(unsigned pageAddr, unsigned* writeData);
int NandEraseBlock(unsigned pageAddr);

MINI2440_STATUS NandTest(int argc, char (*argv)[MAX_COMMAND_LENGTH]);

*/
#endif
