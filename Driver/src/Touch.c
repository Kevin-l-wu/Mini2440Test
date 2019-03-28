#include "Interrupt.h"
#include "Uart.h"

#define ADCCON (*(volatile unsigned long*)0x58000000)
#define ADCTSC (*(volatile unsigned long*)0x58000004)

#define ADCDLY (*(volatile unsigned long*)0x58000008)

#define ADCDAT0 (*(volatile unsigned long*)0x5800000C)
#define ADCDAT1 (*(volatile unsigned long*)0x58000010)

#define ADCUPDN (*(volatile unsigned long*)0x58000014)

int xdata = 0;
int ydata = 0;

void touch_init()
{
	//1. Set AD clock
	ADCCON = (1 << 14) | (49 << 6);
	
	//2. Set interrupt mask
	INTMSK &= ~(1 << 31);
	INTSUBMSK &= ~(1 << 9);
	
	//3. Enter wait interrupt
	ADCTSC = 0xd3;
}

void dump_touch_reg()
{
	printf_string("\n\n-------------- dump_touch_reg --------------------\n\n");
	
	printf_string("ADCCON = 0x%x\n", ADCCON);
	printf_string("ADCTSC = 0x%x\n", ADCTSC);
	
	printf_string("\n\n-------------------------------------------------------\n\n");
}

void touch_handler()
{
	print_string("touch_handler");
	
	//1. Start X/Y coordinate auto convert
	ADCCON |= (1 << 0);
	ADCTSC |= (1 << 2);
	
	//2. Wait conversion finish
	while((ADCCON & (1 << 15)) == 0);
	
	//3. Get X/Y coordinate
	xdata = ADCDAT0 & 0x03ff;
	ydata = ADCDAT1 & 0x03ff;

	//4. Clear interrupt(Write 1)
	(SUBSRCPND) |= (1 << 9);
	(SRCPND) |= (1 << 31);
	(INTPND) |= (1 << 31);
	
	//5. Enter wait stylus up interrupt
	ADCTSC = 0xd3;
	ADCTSC |= (1 << 8);
	
	while(1)
	{
		if((SUBSRCPND) & (1 << 9))
		{
			break;
		}	
	}
	
	printf_string("\n\nxdata = %d\nydata = %d\n\n", xdata, ydata);

	//6. Clear stylus up interrupt
	(SUBSRCPND) |= (1 << 9);
	(SRCPND) |= (1 << 31);
	(INTPND) |= (1 << 31);
	
	//7. Wait another stylus down
	ADCTSC = 0xd3;
}