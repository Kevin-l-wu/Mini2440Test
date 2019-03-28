#include "GlobalDefine.h"
#include "Gpio.h"
#include "Interrupt.h"
#include "Error.h"
#include "Button.h"

#include "ModManager.h"
#include "../Protocol/inc/ButtonProtocol.h"

static volatile unsigned int gButton = 0;


static void InitButtonIrq()
{
	unsigned cpsr_value = 0;
	
	print_string("Button Irq Init\n");
	
	/* Configure GPIOF */
	GPGCON &= ~(0x03 << 0) & (0x03 << 6) & (0x03 << 10) & (0x03 << 12) & (0x03 << 14) & (0x03 << 22);
	GPGCON |= (0x02 << 0) | (0x02 << 6) | (0x02 << 10) | (0x02 << 12) | (0x02 << 14) | (0x02 << 22);
   
   // Do not mask in EINTMASK
	(EINTMASK) &= (~(1<<8)) & (~(1<<11)) & (~(1<<13)) & (~(1<<14) & (~(1<<15)) & (~(1<<19))) ;
	
    // Do not set EINT8_23
    (INTMSK)   &= ~(1<<5);
    
    __asm__( 
    /*IRQ enable*/  
    "mrs r0, cpsr\n"
    "bic r0, r0, #0x80\n"
    "msr cpsr_c, r0\n"
	"mov %[cpsr_value1], r0\n"
    : [cpsr_value1] "=r" (cpsr_value)
    :
    );
	printf_string("Cpsr = 0x%x\n", cpsr_value);
	
	EINTPEND = ~(0x00);
}

static void ClearButtonIrq()
{	
	print_string("Clear Button Irq\n");
	
	/* Configure GPIOF */
	GPFCON &= ~(0x03 << 0) & (0x03 << 6) & (0x03 << 10) & (0x03 << 12) & (0x03 << 14) & (0x03 << 22);
   
   // Do not mask in EINTMASK
	(EINTMASK) |= (1 << 8) | (1 << 11) | (1 << 13) | (1 << 14) | (1 << 15) | (1 << 19) ;
	
    // Do not set EINT8_23
    (INTMSK) |= (1<<5);
	
	EINTPEND = ~(0x00);
}

void button_irq()
{
	unsigned value = 0;
	
	value = (EINTPEND);
	
//	printf_string("vaule(EINTPEND) = 0x%x\n", value);
	
	switch(value)
	{
		case (1 << 8):
			LightLed(0);
			gButton = 1;
			break;
		
		case (1 << 11):
			PutOutLed(0);
			gButton = 2;
			break;
		
		case (1 << 13):
			LightLed(1);
			gButton = 3;
			break;
		
		case (1 << 14):
			PutOutLed(1);
			gButton = 4;
			break;
		
		case (1 << 15):
			LightLed(2);
			gButton = 5;
			break;
		
		case (1 << 19):
			PutOutLed(2);
			gButton = 6;
			break;
			
		default:
			break;
	}
}

void ButtonInit()
{
	gButton = 0;

	InitButtonIrq();
}

void ButtonExit()
{
	ClearButtonIrq();
}

int ButtonHit()
{
	return ((gButton == 0) ? 0 : 1);
}

void ButtonFlush()
{
	gButton = 0;
}

int ButtonGet()
{
	return gButton;
}

ButtonModOps buttonModOps = {
	.ButtonInit = ButtonInit,
	.ButtonExit = ButtonExit,
	.ButtonHit = ButtonHit,
	.ButtonFlush = ButtonFlush,
	.ButtonGet = ButtonGet,
};

MODULE_INSTALL(Button, MOD_BUTTON, 0, &buttonModOps);

