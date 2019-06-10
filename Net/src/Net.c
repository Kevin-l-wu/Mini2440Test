#include "GlobalDefine.h"
#include "Net.h"
#include "Uart.h"

u8 gServerIpAddr[4] = {192, 168, 3, 102};
u8 gServerMacAddr[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

u8 gLocalIpAddr[4] = {192, 168, 3, 50};
u8 gLocalMacAddr[6] = {0x09, 0x08, 0x07, 0x06, 0x05, 0x04};


void UdpProcess(u8* buff, int length)
{
	UDP_HDR* udp_hdr_ptr = (UDP_HDR*)buff;
	
	NET_DBG("UdpProcess()\n");
	
	TftpProcess(buff, length, ADJUST_ENDIAN(udp_hdr_ptr->sport));
}

void IpProcess(u8* arp_buff, int length)
{
	IP_HDR* ip_hdr_ptr = (IP_HDR*)arp_buff;
	
	NET_DBG("IpProcess()\n");
	NET_DBG("ip_hdr_ptr->proto = 0x%x\n", ip_hdr_ptr->proto);
	
	switch(ip_hdr_ptr->proto)
	{
		case PROTO_UDP:
			UdpProcess(arp_buff, length);
			break;
			
		default:
			break;
	}
}

//Interrupt Call Function
int NetHandle(char* rsv_buffer, int length)
{
	ETH_HDR* eth_ptr = (ETH_HDR*)rsv_buffer;
	NET_DBG("NetHandle()\n");
	NET_DBG("eth_ptr->type = %x\n", eth_ptr->type);
	
	switch(ADJUST_ENDIAN(eth_ptr->type))
	{
		case PROTO_ARP:
			ArpProcess(rsv_buffer, length);
			break;

		case PROTO_IP:
			IpProcess(rsv_buffer, length);
			break;

		default:
			break;
	}
	
	return 0;
}