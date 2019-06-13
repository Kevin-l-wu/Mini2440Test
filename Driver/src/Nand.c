#include <stdio.h>
#include "GlobalDefine.h"
#include "Common.h"
#include "Error.h"
#include "Framework.h"
#include "String.h"
#include "Nand.h"

#include "ModManager.h"
#include "../Protocol/inc/NandProtocol.h"


/*******************************************************************
 * Function Name: 	 	SelectChip
 * Inputs : 			None
 * Outputs : 			None.
 * Description:			Select Nand Chip
 * Author:				Kevin
 *******************************************************************/
static void SelectChip()
{
	NFCONT &= (~(1 << 1));
}

/*******************************************************************
 * Function Name: 	 	DeselectChip
 * Inputs : 			None
 * Outputs : 			None.
 * Description:			Deselect Nand Chip
 * Author:				Kevin
 *******************************************************************/
static void DeselectChip()
{
	delay(10);
	NFCONT |= (1 << 1);
}

/*******************************************************************
 * Function Name: 	 	ClearRB
 * Inputs : 			None
 * Outputs : 			None.
 * Description:			Clear RB
 * Author:				Kevin
 *******************************************************************/
static void ClearRB()
{
	delay(10);
	NFSTAT |= 0x04U;
	printf("ClearRB: NFCONT = 0x%x\n", NFCONT);
	printf("ClearRB: NFSTAT = 0x%x\n", NFSTAT);
}

/*******************************************************************
 * Function Name: 	 	SendCommand
 * Inputs : 			None
 * Outputs : 			None.
 * Description:			Send command to Nand
 * Author:				Kevin
 *******************************************************************/
static void SendCommand(unsigned command)
{
	NFCMD = command;
}

/*******************************************************************
 * Function Name: 	 	SendAddress
 * Inputs : 			None
 * Outputs : 			None.
 * Description:			Send address to Nand
 * Author:				Kevin
 *******************************************************************/
static void SendAddress(unsigned address)
{
	NFADDR = address;
}

/*******************************************************************
 * Function Name: 	 	WaitRB
 * Inputs : 			None
 * Outputs : 			None.
 * Description:			Wait RB is not busy
 * Author:				Kevin
 *******************************************************************/
static void WaitRB()
{
	while(!(NFSTAT & 0x04U))
	{
		;
	}
}

static void InitEcc()
{
	NFCONT |= (1 << 4);
}

static void LockMainEcc()
{
	NFCONT |= (1 << 5);
}

static void UnlockMainEcc()
{
	NFCONT &= ~(1 << 5);
}

static void LockSpareEcc()
{
	NFCONT |= (1 << 6);
}

static void UnlockSpareEcc()
{
	NFCONT &= ~(1 << 6);
}

/*******************************************************************
 * Function Name: 	 	NandPageRead
 * Inputs : 			unsigned pageAddr, char* readData
 * Outputs : 			None.
 * Description:			Read one page data from Nand
 * Author:				Kevin
 *******************************************************************/
static void NandPageRead(unsigned pageAddr, unsigned* readData)
{
	int i = 0;
	
	//Select NandFlash
	SelectChip();

	//Clear R/B
	ClearRB();
	
	//Send page read start command 0x00
	SendCommand(0x00);

	//Send column address
	SendAddress(0x00);
	SendAddress(0x00);
	
	//Send row address
	SendAddress(pageAddr & 0xff);
	SendAddress((pageAddr >> 8) & 0xff);
	SendAddress((pageAddr >> 16) & 0xff);
	
	//Send page read end command 0x30
	SendCommand(0x30);
	
	//Wait R/B

	WaitRB();
	
	//Get data
	for(i = 0; i < (BYTES_PER_PAGE / 4); i++)
	{
		readData[i] = NFDATA;
	}

	//Deselect NandFlash
	DeselectChip();
}


/*******************************************************************
 * Function Name: 	 	NandRandomPageRead
 * Inputs : 			unsigned pageAddr, char* readData
 * Outputs : 			None.
 * Description:			Read one page data from Nand
 * Author:				Kevin
 *******************************************************************/

static void NandRandomPageRead(unsigned pageAddr, unsigned columnAddr, unsigned* readData, unsigned length)
{
	int index = 0;
	
	unsigned realReadLenth = length;
	
	//If read data > one page, should reduce length
	if(realReadLenth > (BYTES_PER_PAGE + SPARE_BYTES_PER_PAGE) - columnAddr)
	{
		realReadLenth = (BYTES_PER_PAGE + SPARE_BYTES_PER_PAGE) - columnAddr;
	}
	
	printf("\n\nNandRandomPageRead pageAddress(hex) = 0x%x\n\n", pageAddr);
	printf("\n\nNandRandomPageRead columnAddress(hex) = 0x%x\n\n", columnAddr);
	printf("\n\nNandRandomPageRead realReadLenth(hex) = 0x%x\n\n", realReadLenth);
	
	//Select NandFlash
	SelectChip();

	//Clear R/B
	ClearRB();
	
	//Send page read start command 0x00
	SendCommand(0x00);
	
	//Send column address
	SendAddress(columnAddr & 0xff);
	SendAddress((columnAddr >> 8) & 0xff);
	
	//Send row address
	SendAddress(pageAddr & 0xff);
	SendAddress((pageAddr >> 8) & 0xff);
	SendAddress((pageAddr >> 16) & 0xff);
	
	//Send page read end command 0x30
	SendCommand(0x30);
	
	//Wait R/B
//	printf("NandPageRead before WaitRB: NFSTAT = 0x%x\n", NFSTAT);
//	printf("NandPageRead before WaitRB: NFCONT = 0x%x\n", NFCONT);
	WaitRB();
//	printf("NandPageRead After WaitRB: NFSTAT = 0x%x\n", NFSTAT);
//	printf("NandPageRead After WaitRB: NFCONT = 0x%x\n", NFCONT);

	//Send command 0x05
	SendCommand(0x05);
	
	//Send column address
	SendAddress(columnAddr & 0xff);
	SendAddress((columnAddr >> 8) & 0xff);
	
	//Send command 0xE0
	SendCommand(0xE0);
	
	//Get data
	for(index = 0; index < MAX_MAGNIFICATION(realReadLenth, 4); index++)
	{
		readData[index] = NFDATA;
//		data_adjustment((unsigned*)(&readData[index])); //Adjustment data
	}

	//Deselect NandFlash
	DeselectChip();
}

/*******************************************************************
 * Function Name: 	 	NandPageWrite
 * Inputs : 			unsigned pageAddr, char* writeData
 * Outputs : 			int.
 * Description:			Write one page data to nand
 * Author:				Kevin
 *******************************************************************/
static int NandPageWrite(unsigned int pageAddr, unsigned int* writeData)
{
	int ret;
	int i = 0;

	//Chip select
	SelectChip();
	
	//Clear RB
	ClearRB();
	
	//Send command 0x80
	SendCommand(0x80);
	
	//Send Address
	SendAddress(0x00);
	SendAddress(0x00);
	SendAddress(pageAddr & 0xff);
	SendAddress((pageAddr >> 8) & 0xff);
	SendAddress((pageAddr >> 16) & 0xff);
	
	//Write data
	for(i = 0; i < BYTES_PER_PAGE / 4; i++)
	{
		NFDATA = writeData[i];
	}
	//SendCommand 0x10
	SendCommand(0x10);
	
	//Wait RB
	WaitRB();

	//Send command 0x70
	SendCommand(0x70);

	//Read result;
	ret = NFDATA;

	//Chip deselect
	DeselectChip();
	
	return ret;
}


/*******************************************************************
 * Function Name: 	 	NandRandomPageWrite
 * Inputs : 			unsigned pageAddr, char* readData
 * Outputs : 			None.
 * Description:			Read one page data from Nand
 * Author:				Kevin
 *******************************************************************/

static void NandRandomPageWrite(unsigned pageAddr, unsigned columnAddr, unsigned* writeData, unsigned length)
{
	int index = 0;
	
	unsigned realProgramLenth = length;
	
	//If read data > one page, should reduce length
	if(realProgramLenth > (BYTES_PER_PAGE + SPARE_BYTES_PER_PAGE) - columnAddr)
	{
		realProgramLenth = (BYTES_PER_PAGE + SPARE_BYTES_PER_PAGE) - columnAddr;
	}
	
	printf("\n\npageAddress = 0x%x\n\n", pageAddr);
	printf("\n\ncolumnAddress = 0x%x\n\n", columnAddr);
	printf("\n\nrealProgramLenth = 0x%x\n\n", realProgramLenth);
	
	//Select NandFlash
	SelectChip();

	//Clear R/B
	ClearRB();
	
	//Send page program start command 0x80
	SendCommand(0x80);
	
	//Send column address
	SendAddress(columnAddr & 0xff);
	SendAddress((columnAddr >> 8) & 0xff);
	
	//Send row address
	SendAddress(pageAddr & 0xff);
	SendAddress((pageAddr >> 8) & 0xff);
	SendAddress((pageAddr >> 16) & 0xff);

	//Send command 0x85
	SendCommand(0x85);
	
	//Send column address
	SendAddress(columnAddr & 0xff);
	SendAddress((columnAddr >> 8) & 0xff);
	
	//Get data
	for(index = 0; index < MAX_MAGNIFICATION(realProgramLenth, 4); index++)
	{
		NFDATA = writeData[index];
	}

	//Send command 0x10
	SendCommand(0x10);
	
	//Deselect NandFlash
	DeselectChip();
}

/*******************************************************************
 * Function Name: 	 	NandBlockErase
 * Inputs : 			unsigned long pageAddr
 * Outputs : 			int
 * Description:			Erase the specify nand block
 * Author:				Kevin
 *******************************************************************/
static int NandBlockErase(unsigned pageAddr)
{
	int ret;
	//Chip select
	SelectChip();
	
	//Clear RB
	ClearRB();
	
	//Send command 0x60
	SendCommand(0x60);
	
	//Send page address
	SendAddress(pageAddr & 0xff);
	SendAddress((pageAddr >> 8) & 0xff);
	SendAddress((pageAddr >> 16) & 0xff);

	//Send command 0xd0
	SendCommand(0xd0);
	
	//Wait RB
	WaitRB();
	
	//Send command 0x70
	SendCommand(0x70);
	
	//Read result
	delay(10);
	ret = NFDATA;

	//Chip deselect
	DeselectChip();
	
	return ret;
}

/*******************************************************************
 * Function Name: 	 	NandIdRead
 * Inputs : 			unsigned* chipIDBuf
 * Outputs : 			int
 * Description:			Read nand flash ID
 * Author:				Kevin
 *******************************************************************/
static int NandIdRead(unsigned* chipIDBuf)
{
	int i = 0;
	
	//Chip select
	SelectChip();
	
	//Send Command 0x90
	SendCommand(0x90);
	
	//Send address 0x00
	SendAddress(0x00);

	//Read id data from NFDATA(2 cycle and read 8 Byte, while the id is 5 Byte)
	for(i = 0; i < 2; i++)
	{
		chipIDBuf[i] = NFDATA;
	}
	
	//Chip deselect
	DeselectChip();
	
	return 0;
}

/*******************************************************************
 * Function Name: 	 	NandReset
 * Inputs : 			None
 * Outputs : 			void
 * Description:			Reset nand flash
 * Author:				Kevin
 *******************************************************************/
static void NandReset()
{
	SelectChip();

	ClearRB();
	
	SendCommand(0xff);
	
	WaitRB();
	
	DeselectChip();
}

/*******************************************************************
 * Function Name: 	 	NandControllerInit
 * Inputs : 			None
 * Outputs : 			void
 * Description:			Enable NandFlash controller
 * Author:				Kevin
 *******************************************************************/
static void NandControllerInit()
{
//	printf("\n\nNandControllerInit\n\n");

	/*
	1. NFCONF: 
		Set TACLS , TWRPH0, TWRPH1, duration should >0ns, >12ns, >5ns; While HCLK = 100MHz, One cycle is 10 ns
	2. NFCONT: 
		bit0 set to 1, enable Nand flash controller
		bit1 set to 1, disable chip select
		bit12 set to 0, disable soft lock
	3. Reset Nand flash
	*/
	NFCONF = (TACLS << 12) | (TWRPH0 << 8) | (TWRPH1 << 3) & (~1);
	NFCONT |= 0x11;
	NFCONT &= ~(1 << 12);

	NandReset();
}


static void NandModInit()
{
	NandControllerInit();
}

static void NandModReset()
{
	NandReset();
}

static void NandModClose()
{
	
}

NandModOps nandModOps = {
	.NandModInit = NandModInit,
	.NandReset = NandModReset,
	.NandPageRead = NandPageRead,
	.NandRandomPageRead = NandRandomPageRead,
	.NandPageWrite = NandPageWrite,
	.NandRandomPageWrite = NandRandomPageWrite,
	.NandBlockErase = NandBlockErase,
	.NandIdRead = NandIdRead,
	.NandModClose = NandModClose,
};

MODULE_INSTALL(Nand, MOD_NAND, 0, &nandModOps);
