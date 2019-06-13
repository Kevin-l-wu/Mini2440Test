#include <stdio.h>
#include "GlobalDefine.h"
#include "Error.h"
#include "Interrupt.h"
#include "Dm9000.h"
#include "Uart.h"
#include "Net.h"
#include "Arp.h"

#include "ModManager.h"
#include "../Protocol/inc/DM9000Protocol.h"
#include "CommandManager.h"

extern DM9000ModOps* gDM9000ModOps;

extern u8 gServerIpAddr[4];
extern u8 gServerMacAddr[6];

extern u8 gLocalIpAddr[4];
extern u8 gLocalMacAddr[6];


void ArpRequest()
{
	ARP_HDR arp_send_buf;
	int i = 0;
	
	NET_DBG("ArpRequest\n");
	
	if(gDM9000ModOps == NULL)
	{
		gLocateProtocol(MOD_DM9000, (void*)&gDM9000ModOps);
	}
	printf("gDM9000ModOps = %x\n", gDM9000ModOps);
	if(gDM9000ModOps != NULL)
	{
		for(i = 0; i < 6; i++)
		{
			gServerMacAddr[i] = 0xff;//multicast
		}
		
		/* 1. Construct arp request packet */
		memcpy(arp_send_buf.eth_hdr.d_mac, gServerMacAddr, 6);  	//Ethernet packet Destination mac addr, not known, should all 0xff
		memcpy(arp_send_buf.eth_hdr.s_mac, gLocalMacAddr, 6);			//Ethernet packet Source mac addr
		arp_send_buf.eth_hdr.type = ADJUST_ENDIAN(0x0806);		//Ethernet packet type: Arp 0x0806

		arp_send_buf.hw_type = ADJUST_ENDIAN(0x01);				//Hardware type, ethernet = 1;
		arp_send_buf.protocol = ADJUST_ENDIAN(0x0800);			//IP protocol type
		arp_send_buf.hw_len = 6;									//Ethernet address(Mac addr) is 6
		arp_send_buf.protocol_len = 4;							//Ip address length is 4
		arp_send_buf.opcode = ADJUST_ENDIAN(0x01);				//Opcode = 1, request
		
		memcpy(arp_send_buf.src_mac, gLocalMacAddr, 6);				//Send mac addr
		memcpy(arp_send_buf.src_ip_addr, gLocalIpAddr, 4);
		memcpy(arp_send_buf.dst_mac, gServerMacAddr, 6);				
		memcpy(arp_send_buf.dst_ip_addr, gServerIpAddr, 4);		
		
		/* 2. Send request packet*/
	
		gDM9000ModOps->DM9000Send((u8*)&arp_send_buf, (14 + 28));
	}
}

void ArpProcess(u8* arp_buff, int length)
{
	u32 i = 0;
	ARP_HDR* arp_buff_ptr = (ARP_HDR*)arp_buff;

	ARP_HDR arp_send_buf;
	u8 gServerIpAddr[4];
	u8 gServerMacAddr[6];
	NET_DBG("arp_prcoss\n");
	
	switch(ADJUST_ENDIAN(arp_buff_ptr->opcode))
	{
		case 1:	/* Arp request packet */
			/* 1. Construct arp request packet */
			memcpy(arp_send_buf.eth_hdr.d_mac, arp_buff_ptr->src_mac, 6);
			memcpy(arp_send_buf.eth_hdr.s_mac,arp_buff_ptr->dst_mac, 6);
			arp_send_buf.eth_hdr.type = ADJUST_ENDIAN(0x0806);

			arp_send_buf.hw_type = ADJUST_ENDIAN(0x01);
			arp_send_buf.protocol = ADJUST_ENDIAN(0x0800);
			arp_send_buf.hw_len = 6;
			arp_send_buf.protocol_len = 4;
			arp_send_buf.opcode = ADJUST_ENDIAN(0x02);
			
			memcpy(arp_send_buf.src_mac, gLocalMacAddr, 6);
			memcpy(arp_send_buf.src_ip_addr, gLocalIpAddr, 4);
			
			memcpy(arp_send_buf.dst_mac, arp_buff_ptr->src_mac, 6);				
			memcpy(arp_send_buf.dst_ip_addr, arp_buff_ptr->src_ip_addr, 4);		
			
			/* 2. Send request packet*/
			
			if(gDM9000ModOps == NULL)
			{
				gLocateProtocol(MOD_DM9000, (void*)&gDM9000ModOps);
			}
			
			if(gDM9000ModOps != NULL)
			{
				gDM9000ModOps->DM9000Send((u8*)&arp_send_buf, (14 + 28));
			}
				
			break;

		case 2: /* Arp ack packet */
			memcpy(gServerIpAddr, (*arp_buff_ptr).src_ip_addr, 4);
			memcpy(gServerMacAddr, (*arp_buff_ptr).src_mac, 6);
			
			printf("\nHost ip = ");
			for(i = 0; i < 4; i++)
			{
				printf("%d ", gServerIpAddr[i]);
			}
			
			printf("\nHost mac = ");
			for(i = 0; i < 6; i++)
			{
				printf("%C ", gServerMacAddr[i]);
			}
			printf("\n");
			
			break;
			
		default:
			break;
	}
}



