#ifndef INTERRUPT_H_
#define INTERRUPT_H_

/*interrupt*/
#define GPFCON				(*(volatile unsigned int *)0x56000050)
#define GPFDAT				(*(volatile unsigned int *)0x56000054)

#define SRCPND              (*(volatile unsigned int *)0x4A000000)
#define INTMOD              (*(volatile unsigned int *)0x4A000004)
#define INTMSK              (*(volatile unsigned int *)0x4A000008)
#define PRIORITY            (*(volatile unsigned int *)0x4A00000c)
#define INTPND              (*(volatile unsigned int *)0x4A000010)
#define INTOFFSET           (*(volatile unsigned int *)0x4A000014)
#define SUBSRCPND           (*(volatile unsigned int *)0x4A000018)
#define INTSUBMSK           (*(volatile unsigned int *)0x4A00001c)

#define EINTMASK            (*(volatile unsigned int *)0x560000a4)
#define EINTPEND            (*(volatile unsigned int *)0x560000a8)

typedef enum _IRQ_INDEX
{
	EINT0_IRQ_INDEX 		= 0,
	EINT1_IRQ_INDEX 		= 1,
	EINT2_IRQ_INDEX 		= 2,
	EINT3_IRQ_INDEX 		= 3,
	EINT4_7_IRQ_INDEX 		= 4,
	EINT8_23_IRQ_INDEX 		= 5,
	INT_CAM_IRQ_INDEX 		= 6,
	BATT_FLT_IRQ_INDEX 		= 7,
	INT_TICK_IRQ_INDEX 		= 8,
	INT_WDT_A97_IRQ_INDEX 	= 9,
	INT_TIMER0_IRQ_INDEX 	= 10,
	INT_TIMER1_IRQ_INDEX 	= 11,
	INT_TIMER2_IRQ_INDEX 	= 12,
	INT_TIMER3_IRQ_INDEX 	= 13,
	INT_TIMER4_IRQ_INDEX 	= 14,
	INT_UART2_IRQ_INDEX 	= 15,
	INT_LCD_IRQ_INDEX 		= 16,
	INT_DMA0_IRQ_INDEX 		= 17,
	INT_DMA1_IRQ_INDEX 		= 18,
	INT_DMA2_IRQ_INDEX 		= 19,
	INT_DMA3_IRQ_INDEX 		= 20,
	INT_SDI_IRQ_INDEX 		= 21,
	INT_SPI0_IRQ_INDEX 		= 22,
	INT_UART1_IRQ_INDEX 	= 23,
	INT_NFCON_IRQ_INDEX 	= 24,
	INT_USBD_IRQ_INDEX 		= 25,
	INT_USBH_IRQ_INDEX 		= 26,
	INT_IIC_IRQ_INDEX 		= 27,
	INT_UART0_IRQ_INDEX 	= 28,
	INT_SPI1_IRQ_INDEX 		= 29,
	INT_RTC_IRQ_INDEX 		= 30,
	INT_ADC_IRQ_INDEX 		= 31,
} IRQ_INDEX;

void EnableIrq(int irq);
void DisableIrq(int irq);
void ClearPending(int irq);

void EnableMasterIrq();
void DisableMasterIrq();

void IntRegDump();
#endif