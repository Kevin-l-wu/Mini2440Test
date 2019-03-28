#include "Error.h"
#include "Common.h"
#include "Uart.h"
#include "Framework.h"
#include "Gpio.h"
#include "ModManager.h"

#include "GlobalDefine.h"
#include "../Protocol/inc/LedProtocol.h"


/***********************************************************************
Function Name:  		LedModInit
Input Paramater:		None
Function Description:	Led initialize
************************************************************************/
void LedModInit()
{
	//Set GPIOB output 
	GPBCON &= ~(0xff << 10);
	GPBCON |= (0x55 << 10);
	
	//Set GPIOB led bit 1
	GPBDAT |= (0x0f << 5);
}

/***********************************************************************
Function Name:  		LedPrintChar
Input Paramater:		value
Function Description:	Display low 4 bit of value
************************************************************************/
void LedPrintChar(char value)
{
	GPBDAT = (~(value & 0x0f)) << 5;
}

/***********************************************************************
Function Name:  		LightLed
Input Paramater:		ledNumber
Function Description:	Light the ledNumber led
************************************************************************/
void LightLed(int ledNumber)
{
	unsigned gpbcon_set_value = GPBCON;
	unsigned gpbdat_set_value = GPBDAT;
	
	gpbcon_set_value = gpbcon_set_value & (~(0x00000003 << ((ledNumber + 5) * 2)));
	gpbcon_set_value = gpbcon_set_value | (0x00000001 << ((ledNumber + 5) * 2));
	
	gpbdat_set_value = gpbdat_set_value & (~(0x00000001 << (ledNumber + 5)));
	
	GPBCON = gpbcon_set_value;
	GPBDAT = gpbdat_set_value;
}

/***********************************************************************
Function Name:  		PutOutLed
Input Paramater:		ledNumber
Function Description:	Put out the ledNumber led
************************************************************************/
void PutOutLed(int ledNumber)
{
	unsigned gpbcon_set_value = GPBCON;
	unsigned gpbdat_set_value = GPBDAT;
	
	gpbcon_set_value = gpbcon_set_value & (~(0x00000003 << ((ledNumber + 5) * 2)));
	gpbcon_set_value = gpbcon_set_value | (0x00000001 << ((ledNumber + 5) * 2));
	
	gpbdat_set_value = gpbdat_set_value | (0x00000001 << (ledNumber + 5));
	
	GPBCON = gpbcon_set_value;
	GPBDAT = gpbdat_set_value;
}


LedModOps ledModOps ={
	.LedModInit = LedModInit,
	.LedPrintChar = LedPrintChar,
	.LightLed = LightLed,
	.PutOutLed = PutOutLed,
};

MODULE_INSTALL(Led, MOD_LED, 0, &ledModOps);
