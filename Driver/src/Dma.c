#include <stdio.h>
#include "GlobalDefine.h"
#include "Error.h"
#include "Uart.h"
#include "Dma.h"

#define GPBCON (*(volatile unsigned int*)0x56000010)

char buffer[] = "These string is output through DMA0 channel\n";

DMA_SOURCE gDmaSource[] = {
	{
		.reqSrc		= DMA_REQ_SRC_MEM,   		//Indicate memory
		.dmaChannel	= DMA_CHANNEL_SUM,
		.addr		= (unsigned int*)buffer,
		.busType	= AHB_BUS,
		.isInc		= 1,
		.addrCtlVal	= 0x00,		//Increment and APB
	},
	{
		.reqSrc		= DMA_REQ_SRC_UART0,
		.dmaChannel	= DMA_CHANNEL0,
		.busType	= APB_BUS,
		.addr		= (volatile unsigned int*)&UTXH0,
		.isInc		= 0,
		.addrCtlVal	= 0x03,		//Increment and APB
	},
};

DMA_CHANNEL gDmaChannel[] = {
	{
		.channelIndex 		= 	DMA_CHANNEL0,
		.dconSelValTable 	= 	DCON_SEL_VAL_CHANNEL0,
		.disrcRegAddr 		= 	&DISRC0,
		.disrccRegAddr 		= 	&DISRCC0,
		.didstRegAddr 		= 	&DIDST0,
		.didstcRegAddr 		= 	&DIDSTC0,
		.dconRegAddr 		= 	&DCON0,
		.dstatRegAddr 		= 	&DSTAT0,
		.dcsrcRegAddr 		= 	&DCSRC0,
		.dcdstRegAddr 		= 	&DCDST0,
		.dmasktrigRegAddr 	= 	&DMASKTRIG0,
	},
	{
		.channelIndex 		= 	DMA_CHANNEL1,
		.dconSelValTable 	= 	DCON_SEL_VAL_CHANNEL1,
		.disrcRegAddr	 	= 	&DISRC1,
		.disrccRegAddr 		= 	&DISRCC1,
		.didstRegAddr 		= 	&DIDST1,
		.didstcRegAddr 		= 	&DIDSTC1,
		.dconRegAddr 		= 	&DCON1,
		.dstatRegAddr 		= 	&DSTAT1,
		.dcsrcRegAddr 		= 	&DCSRC1,
		.dcdstRegAddr 		= 	&DCDST1,
		.dmasktrigRegAddr 	= 	&DMASKTRIG1,
	},
	{
		.channelIndex		= 	DMA_CHANNEL2,
		.dconSelValTable 	= 	DCON_SEL_VAL_CHANNEL2,
		.disrcRegAddr 		= 	&DISRC2,
		.disrccRegAddr		=	&DISRCC2,
		.didstRegAddr 		= 	&DIDST2,
		.didstcRegAddr 		= 	&DIDSTC2,
		.dconRegAddr 		= 	&DCON2,
		.dstatRegAddr 		= 	&DSTAT2,
		.dcsrcRegAddr 		= 	&DCSRC2,
		.dcdstRegAddr 		= 	&DCDST2,
		.dmasktrigRegAddr 	= 	&DMASKTRIG2,
	},
	{
		.channelIndex 		= 	DMA_CHANNEL3,
		.dconSelValTable 	= 	DCON_SEL_VAL_CHANNEL3,
		.disrcRegAddr 		= 	&DISRC3,
		.disrccRegAddr 		= 	&DISRCC3,
		.didstRegAddr 		= 	&DIDST3,
		.didstcRegAddr 		= 	&DIDSTC3,
		.dconRegAddr 		= 	&DCON3,
		.dstatRegAddr 		= 	&DSTAT3,
		.dcsrcRegAddr 		= 	&DCSRC3,
		.dcdstRegAddr 		= 	&DCDST3,
		.dmasktrigRegAddr 	= 	&DMASKTRIG3,
	},
};

static void DmaRegDump()
{
	printf_string("DISRC0  = 0x%x\n", DISRC0);
	printf_string("DISRCC0 = 0x%x\n", DISRCC0);

	printf_string("DIDST0  = 0x%x\n", DIDST0);
	printf_string("DIDSTC0 = 0x%x\n", DIDSTC0);

	printf_string("DCON0   = 0x%x\n", DCON0);
	
	printf_string("DSTAT0  = 0x%x\n", DSTAT0);	
	printf_string("DCSRC0  = 0x%x\n", DCSRC0);
	
	printf_string("DMASKTRIG0 = 0x%x\n", DMASKTRIG0);
}

/* Get DMA channel index */
static int FindOutChannelIndex(DMA_CHANNEL_INDEX dmaChannel)
{
	int retIndex = -1;
	int indexChannel = 0;
	
	for(indexChannel = 0; indexChannel < (sizeof(gDmaChannel) / sizeof(gDmaChannel[0])); indexChannel++)
	{
		if(gDmaChannel[indexChannel].channelIndex == dmaChannel)
		{
			retIndex = indexChannel;
		}
	}
	
	return retIndex;
}

/* Get DMA source index */
static int FindOutSrcIndex(DMA_REQ_SRC reqSrc)
{
	int retIndex = -1;
	int indexSrc = 0;
	
	for(indexSrc = 0; indexSrc < (sizeof(gDmaSource) / sizeof(gDmaSource[0])); indexSrc++)
	{
		if(gDmaSource[indexSrc].reqSrc == reqSrc)
		{
			retIndex = indexSrc;
		}
	}
	
	return retIndex;
}

/* Set DMA source address and control value*/
static int DmaSrcSet(DMA_CHANNEL_INDEX dmaChannel, DMA_REQ_SRC reqSrc) 
{
	int ret = 0;
	int indexChannel = -1;
	int indexSrc = -1;
	
	indexChannel = FindOutChannelIndex(dmaChannel);
	indexSrc = FindOutSrcIndex(reqSrc);
	
	if(indexChannel >=0 && indexSrc >= 0)
	{
		*(gDmaChannel[indexChannel].disrcRegAddr) = (volatile unsigned int)(gDmaSource[indexSrc].addr);
		*(gDmaChannel[indexChannel].disrccRegAddr) = (gDmaSource[indexSrc].addrCtlVal & 0x03);	
	}
	else
	{
		printf_string("DmaSrcSet: Invalid DMA channel or src request\n");
		ret = -1;
	}
	
	return ret;
}

/* Set DMA dest address and control value*/
static int DmaDstSet(DMA_CHANNEL_INDEX dmaChannel, DMA_REQ_SRC reqSrc)
{
	int ret = 0;
	int indexChannel = -1;
	int indexSrc = -1;
	
	indexChannel = FindOutChannelIndex(dmaChannel);
	indexSrc = FindOutSrcIndex(reqSrc);
	
	if(indexChannel >=0 && indexSrc >= 0)
	{
		*(gDmaChannel[indexChannel].didstRegAddr) = (volatile unsigned int)(gDmaSource[indexSrc].addr);
		*(gDmaChannel[indexChannel].didstcRegAddr) = (gDmaSource[indexSrc].addrCtlVal & 0x07);	
	}
	else
	{
		printf_string("DmaDstSet: Invalid DMA channel or src request\n");
		ret = -1;
	}
	
	return ret;
}

/* DMA CONT HW select value DCON[26:24] */
static unsigned int DmaHwSel(DMA_CHANNEL_INDEX dmaChannel, DMA_REQ_SRC reqSrc)
{
	unsigned int selVal = 0;
	int indexChannel = -1;
	int indexSrc = -1;
	
	indexChannel = FindOutChannelIndex(dmaChannel);
	
	if(indexChannel >= 0)
	{
		for(indexSrc = 0; indexSrc < REQ_SRC_NUMBER_PER_CHANNLE; indexSrc++)
		{
			if(gDmaChannel[indexChannel].dconSelValTable[indexSrc].reqSrc == reqSrc)
			{
				selVal = gDmaChannel[indexChannel].dconSelValTable[indexSrc].selVal;
			}
		}
	}
	else
	{
		printf_string("DmaHwSel: Invalid DMA channel No.\n");
	}
	
	return selVal;
}

/* DMA CONT Set */
static void DmaCtlSet(DMA_CHANNEL_INDEX dmaChannel,  unsigned int dconVal)
{
	int indexChannel = -1;
	
	indexChannel = FindOutChannelIndex(dmaChannel);
	
	if(indexChannel >= 0)
	{
		*(gDmaChannel[indexChannel].dconRegAddr) = dconVal;
	}
	else
	{
		printf_string("DmaCtlSet: Invalid DMA channel No.\n");
	}
}

/* DMA off on */
static void DmaOffOn(DMA_CHANNEL_INDEX dmaChannel, unsigned char isOn)
{
	int indexChannel = -1;
	
	indexChannel = FindOutChannelIndex(dmaChannel);
		
	if(indexChannel >= 0)
	{
		if(isOn)
		{
			*(gDmaChannel[indexChannel].dmasktrigRegAddr) |= (0x01 << 1);
		}
		else
		{
			*(gDmaChannel[indexChannel].dmasktrigRegAddr) &= (~(0x01 << 1));
		}
	}
	else
	{
		printf_string("DmaOffOn: Invalid DMA channel No.\n");
	}
}


static void DmaStop(DMA_CHANNEL_INDEX dmaChannel)
{
	int indexChannel = -1;
	
	indexChannel = FindOutChannelIndex(dmaChannel);
	
	if(indexChannel >= 0)
	{
		*(gDmaChannel[indexChannel].dmasktrigRegAddr) |= (0x01 << 3);
	}
	else
	{
		printf_string("Invalid DMA channel No.\n");
	}
}

void Test()
{
	unsigned int dconVal = 0;
	
	/* 1. Set sources */
	DmaSrcSet(DMA_CHANNEL0, DMA_REQ_SRC_MEM);

	/* 2. Set dest */
	DmaDstSet(DMA_CHANNEL0, DMA_REQ_SRC_UART0);
	
	/* 3. Set DCON */
	dconVal = DmaHwSel(DMA_CHANNEL0, DMA_REQ_SRC_UART0);
	dconVal |= HW_SOURCE | RELOAD_OFF | DSZ_BYTE | 44;

	DmaCtlSet(DMA_CHANNEL0, dconVal);
	
	/* 4. Start dma */
	UCON0 &= (~(0x03 << 2));
	UCON0 |= (0x02 << 2);
	DmaOffOn(DMA_CHANNEL0, 1);
	
	delay(100000);
	
	/* 5. Stop dma */
	DmaStop(DMA_CHANNEL0);
	DmaOffOn(DMA_CHANNEL0, 0);
}

void dma_init()
{
	GPBCON &= ((~(0x03 << 18)) & (~(0x03 << 20)));
	GPBCON |= ((0x02 << 18) | (0x02 << 20));
	
	/* Set sources address */
	DISRC0 = (unsigned int)buffer;

	//Configure sources address
	DISRCC0 = 0x00;
	
	//set destination address
	DIDST0 = (unsigned long)&UTXH0;

	//Configure destination address
	DIDSTC0 = 0x03;
	
	//Set DCON register
	DCON0 = ((0x01 << 24) | (0x03 << 22) | ((0x0c) << 0));
}


void dma_send_data()
{
	UCON0 &= (~(0x03 << 2));
	UCON0 |= (0x02 << 2);
	
	DMASKTRIG0 = (0x01 << 1);
}

void dma_send_clear()
{
	UCON0 = 5;
	DMASKTRIG0 = 0;
}


MINI2440_STATUS dma_test(int argc, char(*argv)[MAX_COMMAND_LENGTH])
{
	MINI2440_STATUS status = MINI2440_SUCCESS;

	Test();
	
	return status;
}