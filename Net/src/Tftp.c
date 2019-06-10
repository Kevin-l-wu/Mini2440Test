#include <stdio.h>
#include "GlobalDefine.h"
#include "String.h"
#include "Error.h"
#include "Dm9000.h"
#include "Net.h"
#include "Tftp.h"

#include "ModManager.h"
#include "../Protocol/inc/DM9000Protocol.h"
#include "CommandManager.h"

extern DM9000ModOps* gDM9000ModOps;

extern u8 gServerIpAddr[4];
extern u8 gServerMacAddr[6];

extern u8 gLocalIpAddr[4];
extern u8 gLocalMacAddr[6];

#define ADJUST_ENDIAN(val) (((val) & 0x00ff) << 8 | ((val) & 0xff00) >> 8)

static u8 gTftpSendBuf[1024];					//Tftp send data buffer
static u8* gTftpLoadAddr = (u8*)0x30004000;	//Tftp load data address
static unsigned int gTftpLoadLength = 0;		//Tftp load data length
u8 gTftpFinishFlag = 0;					//Tftp load data finish flag

static u8* gFileName = "uImage";

static u16 gCurrentBlockNum = 1;

u16 CheckSum(u8 *ptr, int len)
{
    u32 sum = 0;
    u16 *p = (u16*)ptr;

    while (len > 1)
    {
        sum += *p++;
        len -= 2;
    }
    
    if(len == 1)
        sum += *(u8*)p;
    
    while(sum >> 16)
        sum = (sum & 0xffff) + (sum >> 16);
    
    return (u16)((~sum)&0xffff);
}

void TftpSendRequest()
{
	u8* tftp_ptr = &gTftpSendBuf[200];
	u32 tftp_len = 0;
	UDP_HDR* udp_hdr = 0;
	u8* iphdr = 0;

	gCurrentBlockNum = 1;	//Set current block number to 1
	
	if(gDM9000ModOps == NULL)
	{
		gLocateProtocol(MOD_DM9000, (void*)&gDM9000ModOps);
	}
	
	if(gDM9000ModOps != NULL)
	{
		if(!(gDM9000ModOps->DM9000CheckInit()))
		{
			gDM9000ModOps->DM9000Init();
		}
	
		gTftpLoadLength = 0;
		gTftpFinishFlag = 0;
	
		/* 1. Filled tftp header */
		tftp_ptr[0] = 0x00;
		tftp_ptr[1] = 0x01;
		tftp_len += 2;
		
		memcpy(&tftp_ptr[tftp_len], gFileName, strlen(gFileName));
		tftp_len += strlen(gFileName);
		tftp_ptr[tftp_len] = '\0';
		tftp_len += 1;
		
		memcpy(&tftp_ptr[tftp_len], (char*)("octet"), strlen("octet"));
		tftp_len += strlen("octet");
		tftp_ptr[tftp_len] = '\0';
		tftp_len += 1;
		
		/* 2. Filled udp header */
		udp_hdr = (UDP_HDR*)(tftp_ptr - sizeof(UDP_HDR));
		iphdr = tftp_ptr - sizeof(UDP_HDR) + sizeof(ETH_HDR);

		udp_hdr->sport = ADJUST_ENDIAN(48915);
		udp_hdr->dport = ADJUST_ENDIAN(69);
		udp_hdr->udplen = ADJUST_ENDIAN(tftp_len + sizeof(UDP_HDR) - sizeof(IP_HDR));
		udp_hdr->udpchksum = 0x00;
		
		/* 3. Filled ip header */
		udp_hdr->ip_hdr.vhl = (0x45);
		udp_hdr->ip_hdr.tos = (0x00);
		udp_hdr->ip_hdr.iplen = ADJUST_ENDIAN(tftp_len + sizeof(UDP_HDR) - sizeof(ETH_HDR));
		udp_hdr->ip_hdr.ipid = ADJUST_ENDIAN(0x00);
		udp_hdr->ip_hdr.ipoffset = ADJUST_ENDIAN(0x4000);
		udp_hdr->ip_hdr.ttl = 0xff;
		udp_hdr->ip_hdr.proto = 17;
		
		memcpy(udp_hdr->ip_hdr.src_ip_addr, gLocalIpAddr, 4);
		memcpy(udp_hdr->ip_hdr.dst_ip_addr, gServerIpAddr, 4);
		
		udp_hdr->ip_hdr.ipchksum = 0x00;
		udp_hdr->ip_hdr.ipchksum = CheckSum(iphdr, 20);
		
		/* 4. Filled eth header */
		memcpy(udp_hdr->ip_hdr.eth_hdr.s_mac, gLocalMacAddr, 6);
		memcpy(udp_hdr->ip_hdr.eth_hdr.d_mac, gServerMacAddr, 6);
		
		udp_hdr->ip_hdr.eth_hdr.type = ADJUST_ENDIAN(PROTO_IP);
		
		
		gDM9000ModOps->DM9000Send((u8*)udp_hdr, sizeof(UDP_HDR) + tftp_len);
	}
}

void TftpSetLoadAddr(unsigned char* address)
{	
	if(address != NULL)
	{
		gTftpLoadAddr = address;
	}
}

void TftpSetFileName(unsigned char* fileName)
{
	if(fileName != NULL)
	{
		gFileName = fileName;
	}		
}

void TftpPrintData()
{
	if(gTftpLoadAddr != NULL && gTftpLoadLength > 0)
	{
		PrintAscii(gTftpLoadAddr, gTftpLoadLength, 8);
	}
}

void TftpSendAck(u16 blocknum, u16 port)
{
	u8* tftp_ptr = &gTftpSendBuf[200];
	u32 tftp_len = 0;
	UDP_HDR* udp_hdr = 0;
	u8* iphdr = 0;	
	
	/* 1. Filled tftp header */
	tftp_ptr[0] = 0x00;
	tftp_ptr[1] = 0x04;										//opcode = 4
	tftp_len += 2;
	
	tftp_ptr[2] = (blocknum & 0xff00) >> 8;					//block num
	tftp_ptr[3] = blocknum & 0xff;
	tftp_len += 2;
	
	/* 2. Filled udp header */
	udp_hdr = (UDP_HDR*)(tftp_ptr - sizeof(UDP_HDR));
	iphdr = tftp_ptr - sizeof(UDP_HDR) + sizeof(ETH_HDR);

	udp_hdr->sport = ADJUST_ENDIAN(48915);
	udp_hdr->dport = ADJUST_ENDIAN(port);					//port
	udp_hdr->udplen = ADJUST_ENDIAN(tftp_len + sizeof(UDP_HDR) - sizeof(IP_HDR));
	udp_hdr->udpchksum = 0x00;
	
	/* 3. Filled ip header */
	udp_hdr->ip_hdr.vhl = (0x45);
	udp_hdr->ip_hdr.tos = (0x00);
	udp_hdr->ip_hdr.iplen = ADJUST_ENDIAN(tftp_len + sizeof(UDP_HDR) - sizeof(ETH_HDR));
	udp_hdr->ip_hdr.ipid = ADJUST_ENDIAN(0x00);
	udp_hdr->ip_hdr.ipoffset = ADJUST_ENDIAN(0x4000);
	udp_hdr->ip_hdr.ttl = 0xff;
	udp_hdr->ip_hdr.proto = 17;
	
	memcpy(udp_hdr->ip_hdr.src_ip_addr, gLocalIpAddr, 4);
	memcpy(udp_hdr->ip_hdr.dst_ip_addr, gServerIpAddr, 4);
	
	udp_hdr->ip_hdr.ipchksum = 0x00;
	udp_hdr->ip_hdr.ipchksum = CheckSum(iphdr, 20);
	
	/* 4. Filled eth header */
	memcpy(udp_hdr->ip_hdr.eth_hdr.s_mac, gLocalMacAddr, 6);
	memcpy(udp_hdr->ip_hdr.eth_hdr.d_mac, gServerMacAddr, 6);
	
	udp_hdr->ip_hdr.eth_hdr.type = ADJUST_ENDIAN(PROTO_IP);
	
	if(gDM9000ModOps == NULL)
	{
		gLocateProtocol(MOD_DM9000, (void*)&gDM9000ModOps);
	}
	
	if(gDM9000ModOps != NULL)
	{
		gDM9000ModOps->DM9000Send((u8*)udp_hdr, sizeof(UDP_HDR) + tftp_len);
	}
}

void TftpProcess(u8* buff, int length, u16 port)
{
	/* 1. Check if is tftp data packet */
	TFTP_PACKAGE* tftp_ptr = (TFTP_PACKAGE*)(buff + sizeof(UDP_HDR));
	u32 i = 0;
	u32 tftpDataLen = length - sizeof(UDP_HDR);	
	
	NET_DBG("length = 0x%x\n", length);
	NET_DBG("sizeof(UDP_HDR) = 0x%x\n", sizeof(UDP_HDR));
	NET_DBG("tftpDataLen = 0x%x\n", tftpDataLen);
	
	tftpDataLen = tftpDataLen - 4;	//Deduct the first 4 byte(Block number and opcode)
	
	if(ADJUST_ENDIAN(tftp_ptr->opcode) == 3) 	//tftp data packet
	{
		if(ADJUST_ENDIAN(tftp_ptr->blocknum) == gCurrentBlockNum)
		{
			for(i = 0; i < tftpDataLen; i++)	
			{
				*(gTftpLoadAddr + i + ((gCurrentBlockNum - 1) * 512)) = *(tftp_ptr->data + i);
				gTftpLoadLength++;
			}

			TftpSendAck(ADJUST_ENDIAN(tftp_ptr->blocknum), port);
			gCurrentBlockNum++;
			
			if(tftpDataLen < 512)//Tftp finish flag
			{
				gTftpFinishFlag = 1;
			}
		}
	}
}
