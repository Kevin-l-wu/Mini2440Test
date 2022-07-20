#include "GlobalDefine.h"
#include "Error.h"
#include "Stdio.h"
#include "Timer.h"
#include "Interrupt.h"

#define CONFIG_INTERRUPT_DEBUG

extern void OSTickISR();

/* Dump out main interrupt register */
void IntRegDump()
{
	LogPrintf("\n---------------------- Dump Interrupt Registers -----------------------\n");
	
	LogPrintf("SRCPND = 0x%x\n", SRCPND);
	LogPrintf("INTMSK = 0x%x\n", INTMSK);
	LogPrintf("INTPND = 0x%x\n", INTPND);
	LogPrintf("INTOFFSET = 0x%x\n", INTOFFSET);
	LogPrintf("EINTMASK = 0x%x\n", EINTMASK);
	LogPrintf("EINTPEND = 0x%x\n", EINTPEND);
	
	LogPrintf("\n-----------------------------------------------------------------------\n");	
}

/* Enable irq and mask all interrupt */
void EnableMasterIrq()
{
	/* IRQ enable, set PSR I bit 1 */  
	__asm__( 
    "mrs r0, cpsr\n"
    "bic r0, r0, #0x80\n"
    "msr cpsr_c, r0\n"
    : 
    :
    );
	
	/* Set irq mode */
	INTMOD = 0x00000000;
	/*
	Irq: normal interrupt. 
	Fiq: fast interrupt
	1. Only one interrupt source can be serviced in the FIQ mode
	2. Will not affect both INTPND and INTOFFSET registers
	*/
	
	/* Mask all interrupt */
	INTMSK |= 0xffffffff;
	
	/* Mask all sub interrupt */
	INTSUBMSK |= 0xffffffff;
}

/* Disable irq and mask all interrupt */
void DisableMasterIrq()
{
	/* IRQ disenable, set PSR I bit 1 */  
	__asm__( 
    "mrs r0, cpsr\n"
    "orr r0, r0, #0x80\n"
    "msr cpsr_c, r0\n"
    : 
    :
    );
	
	/* Mask all interrupt */
	INTMSK |= 0xffffffff;
	
	/* Mask all sub interrupt */
	INTSUBMSK |= 0xffffffff;
}

void EnableIrq(int irq)
{
	INTMSK &= ~(0x01 << irq);
}

void DisableIrq(int irq)
{
	INTMSK |= (0x01 << irq);
}

void ClearPending(int irq)
{
	INTPND |= (0x01 << irq);
}

/* Irq main enter */
void HandleIrq()
{
	unsigned intOffsetVal = 0;
	unsigned intMaskVal = 0;
	
	/* Save original Mask, then mask all interrupt */
	intMaskVal = INTMSK;
	INTMSK = 0xffffffff;
	
	LogPrintf("----- Process interrupt -----\n");

	intOffsetVal = (INTOFFSET);
	
#ifdef CONFIG_INTERRUPT_DEBUG	
	LogPrintf("intOffsetVal = 0x%x\n", intOffsetVal);
	IntRegDump();
#endif

	switch(intOffsetVal)
	{
		case EINT0_IRQ_INDEX:			/* 0 */
			break;
		case EINT1_IRQ_INDEX:			/* 1 */
			break;
		case EINT2_IRQ_INDEX:			/* 2 */
			break;
		case EINT3_IRQ_INDEX:			/* 3 */
			break;
			
		case EINT4_7_IRQ_INDEX: 		/* 4 */
//			DM9000IntProcess();
			break;
		case EINT8_23_IRQ_INDEX:		/* 5 */
			button_irq();
			break;
		case INT_CAM_IRQ_INDEX:			/* 6 */
			break;
		case BATT_FLT_IRQ_INDEX:		/* 7 */
			break;
		
		case INT_TICK_IRQ_INDEX:		/* 8 */
			break;
		case INT_WDT_A97_IRQ_INDEX:		/* 9 */
			break;
		case INT_TIMER0_IRQ_INDEX:		/* 10 */
			Timer0Interrupt();
			
			break;
		case INT_TIMER1_IRQ_INDEX:		/* 11 */
			break;
	
		case INT_TIMER2_IRQ_INDEX:		/* 12 */
			break;
		case INT_TIMER3_IRQ_INDEX:		/* 13 */
			break;
		case INT_TIMER4_IRQ_INDEX:		/* 14 */
//			Timer4Interrupt();
			OSTickISR();
			break;
		case INT_UART2_IRQ_INDEX:		/* 15 */
			break;
		
		case INT_LCD_IRQ_INDEX:			/* 16 */
			break;
		case INT_DMA0_IRQ_INDEX:		/* 17 */
			break;
		case INT_DMA1_IRQ_INDEX:		/* 18 */
			break;
		case INT_DMA2_IRQ_INDEX:		/* 19 */
			break;
		
		case INT_DMA3_IRQ_INDEX:		/* 20 */
			break;
		case INT_SDI_IRQ_INDEX:			/* 21 */
			break;
		case INT_SPI0_IRQ_INDEX:		/* 22 */
			break;
		case INT_UART1_IRQ_INDEX:		/* 23 */
			break;
		
		case INT_NFCON_IRQ_INDEX:		/* 24 */
			break;
		case INT_USBD_IRQ_INDEX:		/* 25 */
			break;
		case INT_USBH_IRQ_INDEX:		/* 26 */
			break;
		case INT_IIC_IRQ_INDEX:			/* 27 */
			break;
		
		case INT_UART0_IRQ_INDEX:		/* 28 */
			break;
		case INT_SPI1_IRQ_INDEX:		/* 29 */
			break;
		case INT_RTC_IRQ_INDEX:			/* 30 */
			break;
		case INT_ADC_IRQ_INDEX:			/* 31 */
			touch_handler();
			break;
		
		default:
			break;
	}	
	
	(SUBSRCPND) |= ~(0x00);
	(SRCPND) |= ~(0x00);
	(INTPND) |= ~(0x00);
	(EINTPEND) |= ~(0x00);

	INTMSK = intMaskVal;
}