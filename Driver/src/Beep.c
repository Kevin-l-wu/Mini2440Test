#include "Error.h"
#include "Common.h"
#include "Uart.h"
#include "Framework.h"
#include "Gpio.h"
#include "ModManager.h"

#include "GlobalDefine.h"
#include "../Protocol/inc/BeepProtocol.h"

void BeepInit()
{
	//Set GPIOB output 
	GPBCON &= ~(0x03 << 0);
	GPBCON |= (0x01 << 0);
}

void BeepEnable()
{
	GPBDAT |= (0x01 << 0);
}

void BeepDisable()
{
	GPBDAT &= ~(0x01 << 0);
}

BeepModOps beepModOps ={
	.BeepInit = BeepInit,
	.BeepEnable = BeepEnable,
	.BeepDisable = BeepDisable,
};

MODULE_INSTALL(Beep, MOD_BEEP, 0, &beepModOps);
