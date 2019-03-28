#ifndef DMA_H_
#define DMA_H_

/************************** Dma channel registers define *****************************/

/* Dma channel 1 registers define */

#define DISRC0 	(*(volatile unsigned int*)0x4B000000)
#define DISRCC0 (*(volatile unsigned int*)0x4B000004)

#define DIDST0 	(*(volatile unsigned int*)0x4B000008)
#define DIDSTC0 (*(volatile unsigned int*)0x4B00000C)

#define DCON0 	(*(volatile unsigned int*)0x4B000010)

#define DSTAT0 	(*(volatile unsigned int*)0x4B000014)

#define DCSRC0 	(*(volatile unsigned int*)0x4B000018)
#define DCDST0 	(*(volatile unsigned int*)0x4B00001C)

#define DMASKTRIG0 	(*(volatile unsigned int*)0x4B000020)


/* Dma channel 1 registers define */

#define DISRC1 	(*(volatile unsigned int*)0x4B000040)
#define DISRCC1 (*(volatile unsigned int*)0x4B000044)

#define DIDST1 	(*(volatile unsigned int*)0x4B000048)
#define DIDSTC1 (*(volatile unsigned int*)0x4B00004C)

#define DCON1 	(*(volatile unsigned int*)0x4B000050)

#define DSTAT1 	(*(volatile unsigned int*)0x4B000054)

#define DCSRC1 	(*(volatile unsigned int*)0x4B000058)
#define DCDST1 	(*(volatile unsigned int*)0x4B00005C)

#define DMASKTRIG1 	(*(volatile unsigned int*)0x4B000060)


/* Dma channel 2 registers define */

#define DISRC2 	(*(volatile unsigned int*)0x4B000080)
#define DISRCC2 (*(volatile unsigned int*)0x4B000084)

#define DIDST2 	(*(volatile unsigned int*)0x4B000088)
#define DIDSTC2 (*(volatile unsigned int*)0x4B00008C)

#define DCON2 	(*(volatile unsigned int*)0x4B000090)

#define DSTAT2 	(*(volatile unsigned int*)0x4B000094)

#define DCSRC2 	(*(volatile unsigned int*)0x4B000098)
#define DCDST2 	(*(volatile unsigned int*)0x4B00009C)

#define DMASKTRIG2 	(*(volatile unsigned int*)0x4B0000A0)


/* Dma channel 3 registers define */

#define DISRC3 	(*(volatile unsigned int*)0x4B0000C0)
#define DISRCC3 (*(volatile unsigned int*)0x4B0000C4)

#define DIDST3 	(*(volatile unsigned int*)0x4B0000C8)
#define DIDSTC3 (*(volatile unsigned int*)0x4B0000CC)

#define DCON3 	(*(volatile unsigned int*)0x4B0000D0)

#define DSTAT3 	(*(volatile unsigned int*)0x4B0000D4)
#define DCSRC3 	(*(volatile unsigned int*)0x4B0000D8)
#define DCDST3 	(*(volatile unsigned int*)0x4B0000DC)

#define DMASKTRIG3 	(*(volatile unsigned int*)0x4B0000E0)

/***************************************************************************************/

#define REQ_SRC_NUMBER_PER_CHANNLE 7

/* DCON set value */
#define DEMAND_MODE 		(0 << 31)
#define HANDSHAKE_MODE		(1 << 31)

#define SYNC_APB 			(0 << 30)
#define SYNC_AHB 			(1 << 30)

#define CURR_TC_INT_ENABLE 	(0 << 29)
#define CURR_TC_INT_DISABLE (1 << 29)

#define TSZ_UINT 			(0 << 28)
#define TSZ_BURST 			(1 << 28)

#define SINGLE_SERV_MODE 	(0 << 27)
#define WHOLE_SERV_MODE 	(1 << 27)

#define SW_SOURCE	 		(0 << 23)
#define HW_SOURCE		 	(1 << 23)

#define RELOAD_ON	 		(0 << 22)
#define RELOAD_OFF		 	(1 << 22)

#define DSZ_BYTE			(0b00 << 20)
#define DSZ_HW				(0b01 << 20)
#define DSZ_WORD			(0b10 << 20)

typedef enum _DMA_CHANNEL_INDEX
{
	DMA_CHANNEL0 = 0,
	DMA_CHANNEL1,
	DMA_CHANNEL2,
	DMA_CHANNEL3,
	DMA_CHANNEL_SUM,
} DMA_CHANNEL_INDEX;

typedef enum _DMA_REQ_SRC
{
	DMA_REQ_SRC_NXDREQ0 = 0,
	DMA_REQ_SRC_UART0,
	DMA_REQ_SRC_SDI,
	DMA_REQ_SRC_TIMER,
	DMA_REQ_SRC_USB_DEV_EP1,
	DMA_REQ_SRC_I2SSDO,
	DMA_REQ_SRC_PCMIN,
	DMA_REQ_SRC_NXDREQ1,
	DMA_REQ_SRC_UART1,
	DMA_REQ_SRC_I2SSDI,
	DMA_REQ_SRC_SPI0,
	DMA_REQ_SRC_USB_DEV_EP2,
	DMA_REQ_SRC_PCMOUT,
	DMA_REQ_SRC_USB_DEV_EP3,
	DMA_REQ_SRC_MICIN,
	DMA_REQ_SRC_UART2,
	DMA_REQ_SRC_SPI1,
	DMA_REQ_SRC_USB_DEV_EP4,
	DMA_REQ_SRC_MEM,
	DMA_REQ_SRC_SUM,
} DMA_REQ_SRC;

typedef struct _DCON_SEL_VALUE
{
	DMA_REQ_SRC reqSrc;
	unsigned selVal;
} DCON_SEL_VALUE;


typedef struct _DMA_CHANNEL
{
	DMA_CHANNEL_INDEX channelIndex;
	DCON_SEL_VALUE dconSelValTable[REQ_SRC_NUMBER_PER_CHANNLE];
	volatile unsigned int* disrcRegAddr;
	volatile unsigned int* disrccRegAddr;
	volatile unsigned int* didstRegAddr;
	volatile unsigned int* didstcRegAddr;
	volatile unsigned int* dconRegAddr;
	volatile unsigned int* dstatRegAddr;
	volatile unsigned int* dcsrcRegAddr;
	volatile unsigned int* dcdstRegAddr;
	volatile unsigned int* dmasktrigRegAddr;
} DMA_CHANNEL;

typedef enum _BUS_TYPE
{
	AHB_BUS = 0,
	APB_BUS,
} BUS_TYPE;

typedef enum _INTR_OCCUR
{
	TC0 = 0,
	AUTO_RELOAD = 1,
} INTR_OCCUR;

typedef struct _DMA_SOURCE
{
	DMA_REQ_SRC reqSrc;
	DMA_CHANNEL dmaChannel;
	BUS_TYPE busType;
	volatile unsigned int* addr;
	unsigned char isInc;
	unsigned char addrCtlVal;
} DMA_SOURCE;

#define DCON_SEL_VAL_CHANNEL0 {\
	{ DMA_REQ_SRC_NXDREQ0, 		(0b000 << 24) },\
	{ DMA_REQ_SRC_UART0, 		(0b001 << 24) },\
	{ DMA_REQ_SRC_SDI, 			(0b010 << 24) },\
	{ DMA_REQ_SRC_TIMER, 		(0b011 << 24) },\
	{ DMA_REQ_SRC_USB_DEV_EP1,	(0b100 << 24) },\
	{ DMA_REQ_SRC_I2SSDO, 		(0b101 << 24) },\
	{ DMA_REQ_SRC_PCMIN, 		(0b110 << 24) },\
}


#define DCON_SEL_VAL_CHANNEL1 {}
#define DCON_SEL_VAL_CHANNEL2 {}
#define DCON_SEL_VAL_CHANNEL3 {}


MINI2440_STATUS dma_test(int argc, char(*argv)[MAX_COMMAND_LENGTH]);

#endif