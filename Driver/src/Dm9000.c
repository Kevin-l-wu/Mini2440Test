#include <stdio.h>
#include "GlobalDefine.h"
#include "Error.h"
#include "Uart.h"
#include "Interrupt.h"
#include "Error.h"
#include "String.h"
#include "Dm9000.h"

#include "ModManager.h"
#include "../Protocol/inc/DM9000Protocol.h"


//#define CONFIG_DM9000_DEBUG

#ifdef CONFIG_DM9000_DEBUG
#define DM9000_DBG(fmt,args...) printf_string(fmt, ##args)
#else
#define DM9000_DBG(fmt,args...)
#endif

extern u8 gLocalMacAddr[6];
static u8 dm9000InitFlag = 0;

#define ADJUST_ENDIAN(val) (((val) & 0x00ff) << 8 | ((val) & 0xff00) >> 8) 

static void DM9000_iow(u16 reg, u16 data)
{
	DM_ADD = reg;
	DM_DAT = data;
}

static u8 DM9000_ior(u16 reg)
{
	DM_ADD = reg;
	return DM_DAT;
}

#ifdef CONFIG_DM9000_DEBUG
static void dm9000_dump_register(void)
{
	DM9000_DBG("\n");
	DM9000_DBG("NCR   (0x00): %x\n", DM9000_ior(0));
	DM9000_DBG("NSR   (0x01): %x\n", DM9000_ior(1));
	DM9000_DBG("TCR   (0x02): %x\n", DM9000_ior(2));
	DM9000_DBG("TSRI  (0x03): %x\n", DM9000_ior(3));
	DM9000_DBG("TSRII (0x04): %x\n", DM9000_ior(4));
	DM9000_DBG("RCR   (0x05): %x\n", DM9000_ior(5));
	DM9000_DBG("RSR   (0x06): %x\n", DM9000_ior(6));
	DM9000_DBG("ISR   (0xFE): %x\n", DM9000_ior(DM9000_ISR));
	DM9000_DBG("\n");
}
#endif

static void DM9000CSInit()
{
	/* DM9000 data bus is connected to data bus, 16 bit */
	/* Set bank4 data width = 16 bit, BWSCON[17:16] = 01 */
	BWSCON = BWSCON & (~(0x03 << 16));
	BWSCON = BWSCON | (0x01 << 16);
	
	BANKCON4 = (B4_Tacs << 13) | (B4_Tcos << 11) | (B4_Tacc << 8)
		| (B4_Tcoh << 6) | (B4_Tah << 4) | (B4_Tacp << 2) | (B4_PMC << 0);	
}

static void DM9000IntInit()
{
	print_string("DM9000IntInit\n");
	
	/* DM9000 INT is EINT7 -> GPF[7] */
	/*Configure GPF[7] is EINT7*/
	GPFCON = (GPFCON) & (~(0x3 << 14));
	GPFCON = (GPFCON) | (0x2 << 14);
	
	/* Set EINT7 high level valid */
	EXTINT0 = EXTINT0 & (~(0x07 << 28));
	EXTINT0 = EXTINT0 | (0x01 << 28);

	/* Set INTMASK */
	INTMSK = (INTMSK) & (~(0x01 << 4));
	
	/* Set EINTMASK */
	EINTMASK = (EINTMASK) & (~(0x01 << 7));
	
	DM9000_iow(DM9000_ISR, IMR_PTM);/* Clear Tx bit in ISR */
	DM9000_iow(DM9000_ISR, IMR_PRM);/* Clear Rx bit in ISR */
	
	/* Clear SRCPND and INTPND */
	EINTPEND = (0x01 << 7);
	SRCPND = (0x01 << 4);
	INTPND = (0x01 << 4);
}

static void DM9000IntClear()
{
	print_string("DM9000IntClear\n");
	
	/* DM9000 INT is EINT7 -> GPF[7] */
	/*Configure GPF[7] is EINT7*/
	GPFCON = (GPFCON) & (~(0x3 << 14));
	
	/* Set EINT7 high level valid */
	EXTINT0 = EXTINT0 & (~(0x07 << 28));

	/* Set INTMASK */
	INTMSK = (INTMSK) | (0x01 << 4);
	
	/* Set EINTMASK */
	EINTMASK = (EINTMASK) | (0x01 << 7);
	
	/* Clear SRCPND and INTPND */
	EINTPEND = (0x01 << 7);
	SRCPND = (0x01 << 4);
	INTPND = (0x01 << 4);

}

static void DM9000Reset()
{
	u8 status = 0;
	print_string("DM9000Reset\n");
	
	/* Make all GPIO0 outputs, all others inputs */
	DM9000_iow(DM9000_GPCR, GPCR_GPIO0_OUT);
	
	/* Step 1: Power internal PHY by writing 0 to GPIO0 pin */
	DM9000_iow(DM9000_GPR, 0);
	
	/* Step 2: Software reset */
	DM9000_iow(DM9000_NCR, (NCR_LBK_INT_MAC | NCR_RST));
	
	do {
		DM9000_DBG("resetting the DM9000, 1st reset\n");
		delay(250); /* Wait at least 20 us */
	} while (DM9000_ior(DM9000_NCR) & 1);
	
	
	DM9000_iow(DM9000_NCR, 0);
	DM9000_iow(DM9000_NCR, (NCR_LBK_INT_MAC | NCR_RST)); /* Issue a second reset */
	
	do {
		DM9000_DBG("resetting the DM9000, 2nd reset\n");
		delay(250); /* Wait at least 20 us */
	} while (DM9000_ior(DM9000_NCR) & 1);
	
	status = DM9000_ior(DM9000_ISR);
	printf_string("status = %x\n", status);
}

static int DM9000Probe()
{
	u32 id_val = 0;
	u32 chip_rev = 0;
	
	id_val = DM9000_ior(DM9000_VIDL);
	
	id_val |= DM9000_ior(DM9000_VIDH) << 8;
	
	id_val |= DM9000_ior(DM9000_PIDL) << 16;
	
	id_val |= DM9000_ior(DM9000_PIDH) << 24;
	
	chip_rev |= DM9000_ior(DM9000_CHIPR);

	if (id_val == DM9000_ID)
	{
		printf_string("dm9000 probed, info as below:\n");
		printf_string("dm9000 i/o: 0x%x \n", CONFIG_DM9000_BASE);
		printf_string("dm9000 id: 0x%x \n", id_val);
		printf_string("dm9000 chip_rev: 0x%x \n", chip_rev);
		return 0;
	}
	else
	{
		printf_string("dm9000  not probed\n");
		return -1;
	}
}

static int DM9000Init()
{
	u32 i = 0;
	u32 status = 0;
	
	print_string("---------------- DM9000Init -----------------\n");
	/* 1. Select Dm9000 chip */
	DM9000CSInit();
		
	/* 3. Reset device */
	DM9000Reset();
	
	/* 2. Interrupt initialize */
//	DM9000IntInit();
	
	/* 4. Search Dm9000 */
	DM9000Probe();
	
	/* 5. Mac initialize */
	/* Program operating register, only internal phy supported */
	DM9000_iow(DM9000_NCR, 0x0);
	/* TX Polling clear */
	DM9000_iow(DM9000_TCR, 0);
	/* Less 3Kb, 200us */
	DM9000_iow(DM9000_BPTR, BPTR_BPHW(3) | BPTR_JPT_600US);
	/* Flow Control : High/Low Water */
	DM9000_iow(DM9000_FCTR, FCTR_HWOT(3) | FCTR_LWOT(8));
	/* SH FIXME: This looks strange! Flow Control */
	DM9000_iow(DM9000_FCR, 0x0);
	/* Special Mode */
	DM9000_iow(DM9000_SMCR, 0);
	/* clear TX status */
	DM9000_iow(DM9000_NSR, NSR_WAKEST | NSR_TX2END | NSR_TX1END);
	/* Clear interrupt status */
	DM9000_iow(DM9000_ISR, ISR_ROOS | ISR_ROS | ISR_PTS | ISR_PRS);
	
	/* 6. Fill device MAC address */
	for (i = 0; i < 6; i++)
		DM9000_iow(DM9000_PAR + i, gLocalMacAddr[i]);
	
	/* 7. Active Dm9000 */
	/* RX enable */
	DM9000_iow(DM9000_RCR, RCR_DIS_LONG | RCR_DIS_CRC | RCR_RXEN);

	/* Enable TX/RX interrupt mask */
	DM9000_iow(DM9000_IMR, IMR_PAR);
	
	dm9000InitFlag = 1;	//Set the dm9000 init flag
	
	print_string("---------------- DM9000Init complete -----------------\n");
	return 0;
}


/*
   Write a word to phyxcer
*/
static void
dm9000_phy_write(int reg, u16 value)
{

	/* Fill the phyxcer register into REG_0C */
	DM9000_iow(DM9000_EPAR, DM9000_PHY | reg);

	/* Fill the written data into REG_0D & REG_0E */
	DM9000_iow(DM9000_EPDRL, (value & 0xff));
	DM9000_iow(DM9000_EPDRH, ((value >> 8) & 0xff));
	DM9000_iow(DM9000_EPCR, 0xa);	/* Issue phyxcer write command */
	delay(5000);			/* Wait write complete */
	DM9000_iow(DM9000_EPCR, 0x0);	/* Clear phyxcer write command */
	DM9000_DBG("dm9000_phy_write(reg:0x%x, value:0x%x)\n", reg, value);
}

/*
  Stop the interface.
  The interface is stopped when it is brought.
*/
static void DM9000Halt()
{
	DM9000_DBG("%s\n", __func__);

	/* RESET devie */
	dm9000_phy_write(0, 0x8000);	/* PHY RESET */
	DM9000_iow(DM9000_GPR, 0x01);	/* Power-Down PHY */
	DM9000_iow(DM9000_IMR, 0x80);	/* Disable all interrupt */
	DM9000_iow(DM9000_RCR, 0x00);	/* Disable RX */
}

static void DM9000DeInit()
{
	print_string("---------------- DM9000DeInit -----------------\n");
	
	DM9000Halt();
	
	dm9000InitFlag = 0;	//Clear the dm9000 init flag
}


static u8 DM9000CheckInit()
{
	return dm9000InitFlag;
}

/*After send finish, not interrupt, need mask send interrupt */
static void DM9000Send(u8* data, u32 length)
{
	u32 i = 0;
	u32 status = 0;
	u8 isr_status = 0;
	
//	print_string("DM9000Send\n");
	
	DM9000_iow(DM9000_ISR, IMR_PTM);/* Clear Tx bit in ISR */
	
	/* 1. Mask DM9000 interrupt */
	DM9000_iow(DM9000_IMR, 0x80);
	
	/* 2. Write data which for sending to data*/
	DM_ADD = DM9000_MWCMD;
	
	for(i = 0; i < length; i+=2)
	{
		DM_DAT = data[i] | (data[i+1] << 8);
	}
	
	/* 3. Set TX length to DM9000 */
	DM9000_iow(DM9000_TXPLL, length & 0xff);
	DM9000_iow(DM9000_TXPLH, (length >> 8) & 0xff);
	
	/* 4. Start send*/
	DM9000_iow(DM9000_TCR, TCR_TXREQ); /* Cleared after TX complete */
	
	/* 5. Wait for ending of send*/
	while(1)
	{	
		if((status & 0x01) == 0x00)
		{
			break;
		}
	}
	
	/* 6. Clear send status */
	DM9000_iow(DM9000_NSR, 0x2c);
	
	/* 7. Resume interrupt */
	DM9000_iow(DM9000_IMR, 0x81);
	
	isr_status = DM9000_ior(DM9000_ISR);
	
	DM9000_iow(DM9000_ISR, IMR_PTM);/* Clear Tx bit in ISR */
}

static int DM9000Recv()
{
	u16 status, length;
	u16 tmp;
	u32 i = 0;
	u8 ready = 0;
	u8 isr_status = 0;
	
	u8 recvBuf[1600] = {0};
	
	/* 1. Check if interrupt accrue */
	if(!(DM9000_ior(DM9000_ISR) & 0x01))
		return 0;
	
	DM9000_iow(DM9000_ISR, 0x01);/* clear PR status latched in bit 0 */
	
	for(;;)
	{
		memset(recvBuf, 0, 1600);
		/* 2. Dummy read */
		ready = DM9000_ior(DM9000_MRCMDX);
		
		if((ready & DM9000_PKT_RDY) != DM9000_PKT_RDY)
		{
			return 0;
		}

		/* 3. Read status and get packet length */
		status = DM9000_ior(DM9000_MRCMD);

		/* 4. Read packed length */
		length = DM_DAT;
		
		/* 5. Read packed data */
		if(length < PTK_MAX_LEN)
		{
			for(i = 0; i < length; i += 2)
			{
				tmp = DM_DAT;
				recvBuf[i] = tmp & 0x0ff;
				recvBuf[i + 1] = (tmp >> 8) & 0x0ff;
			}
		   
			if(length != 0)
			{
				NetHandle(recvBuf, length - 4);//Deduct the length of the first 4 bytes(0x01, status, length)
			}
		}
		
#ifdef CONFIG_DM9000_DEBUG	
		printf_string("length = %x\n", length);
		
		print_string("----------- Dump receive data -----------\n");
		
		PrintAscii(recvBuf, length, 40);
		
		print_string("----------- Dump over -----------\n");
#endif	
	}

	return length;
}

/* Interrupt call function */
void DM9000IntProcess()
{
	u32 length = 0;
	u32 isr_status = 0;
	
	u8 rsv_buffer[1500] = {0};
	
#ifdef CONFIG_DM9000_DEBUG		
	print_string("DM9000IntProcess\n");
	dm9000_dump_register();
#endif	
	memset(rsv_buffer, 0, 1500);
	
//	length = DM9000Recv(rsv_buffer);
	
	DM9000_iow(DM9000_ISR, 0x01); /* clear PR status latched in bit 0 */
	DM9000_iow(DM9000_ISR, 0x02); /* clear PR status latched in bit 1 */

	SRCPND = (1 << 4);
	INTPND = (1 << 4);
	EINTPEND = (1 << 7);
}

DM9000ModOps dm9000ModOps = {
	.DM9000Init = DM9000Init,
	.DM9000DeInit = DM9000DeInit,
	.DM9000CheckInit = DM9000CheckInit,
	.DM9000Send = DM9000Send,
	.DM9000Recv = DM9000Recv,
};

MODULE_INSTALL(DM9000, MOD_DM9000, 0, &dm9000ModOps);

