#include "GlobalDefine.h"
#include "String.h"
#include "Error.h"
#include "Dm9000.h"
#include "Arp.h"
#include "Tftp.h"

#define ADJUST_ENDIAN(val) (((val) & 0x00ff) << 8 | ((val) & 0xff00) >> 8)

u8 tftp_send_buff[1024];
u8* tftp_down_addr = (u8*)0x31000000;

u16 checksum(u8 *ptr, int len)
{
    u32 sum = 0;
    u16 *p = (u16 *)ptr;

    while (len > 1)
    {
        sum += *p++;
        len -= 2;
    }
    
    if(len == 1)
        sum += *(u8 *)p;
    
    while(sum>>16)
        sum = (sum&0xffff) + (sum>>16);
    
    return (u16)((~sum)&0xffff);
}

void tftp_send_request(char* filename)
{
	u8* tftp_ptr = &tftp_send_buff[200];
	u32 tftp_len = 0;
	UDP_HDR* udp_hdr = 0;
	u8* iphdr = 0;
	u8 dev_mac_addr[6] = {9, 8, 7, 6, 5, 4};
	u8 host_mac_addr[6] = {0x00, 0x0c, 0x29, 0x75, 0x8f, 0x28};
	
	u8 dev_ip_addr[4] = {10, 0, 0, 8}; 
	u8 host_ip_addr[4] = {10, 0, 0, 20};
	
	/* 1. Filled tftp header */
	tftp_ptr[0] = 0x00;
	tftp_ptr[1] = 0x01;
	tftp_len += 2;
	
	memcpy(&tftp_ptr[tftp_len], filename, strlen(filename));
	tftp_len += strlen(filename);
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
	
	memcpy(udp_hdr->ip_hdr.src_ip_addr, dev_ip_addr, 4);
	memcpy(udp_hdr->ip_hdr.dst_ip_addr, host_ip_addr, 4);
	
	udp_hdr->ip_hdr.ipchksum = 0x00;
	udp_hdr->ip_hdr.ipchksum = checksum(iphdr, 20);
	
	/* 4. Filled eth header */
	memcpy(udp_hdr->ip_hdr.eth_hdr.s_mac, dev_mac_addr, 6);
	memcpy(udp_hdr->ip_hdr.eth_hdr.d_mac, host_mac_addr, 6);
	
	udp_hdr->ip_hdr.eth_hdr.type = ADJUST_ENDIAN(PROTO_IP);
	
	dm9000_send((u8*)udp_hdr, sizeof(UDP_HDR) + tftp_len);
}

void tftp_send_ack(u16 blocknum, u16 port)
{
	u8* tftp_ptr = &tftp_send_buff[200];
	u32 tftp_len = 0;
	UDP_HDR* udp_hdr = 0;
	u8* iphdr = 0;
	u8 dev_mac_addr[6] = {9, 8, 7, 6, 5, 4};
	u8 host_mac_addr[6] = {0x00, 0x0c, 0x29, 0x75, 0x8f, 0x28};
	
	u8 dev_ip_addr[4] = {10, 0, 0, 8}; 
	u8 host_ip_addr[4] = {10, 0, 0, 20};
	
//	print_string("tftp_send_ack\n");
	
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
	
	memcpy(udp_hdr->ip_hdr.src_ip_addr, dev_ip_addr, 4);
	memcpy(udp_hdr->ip_hdr.dst_ip_addr, host_ip_addr, 4);
	
	udp_hdr->ip_hdr.ipchksum = 0x00;
	udp_hdr->ip_hdr.ipchksum = checksum(iphdr, 20);
	
	/* 4. Filled eth header */
	memcpy(udp_hdr->ip_hdr.eth_hdr.s_mac, dev_mac_addr, 6);
	memcpy(udp_hdr->ip_hdr.eth_hdr.d_mac, host_mac_addr, 6);
	
	udp_hdr->ip_hdr.eth_hdr.type = ADJUST_ENDIAN(PROTO_IP);
	
	dm9000_send((u8*)udp_hdr, sizeof(UDP_HDR) + tftp_len);
}

void tftp_process(u8* buff, int length, u16 port)
{
	/* 1. Check if is tftp data packet */
	TFTP_PACKAGE* tftp_ptr = (TFTP_PACKAGE*)(buff + sizeof(UDP_HDR));
	u32 i = 0;
	u32 tftp_len = length - sizeof(UDP_HDR);
	static u16 current_block_num = 1;
	
	if(ADJUST_ENDIAN(tftp_ptr->opcode) == 3) 	//tftp data packet
	{
		if(ADJUST_ENDIAN(tftp_ptr->blocknum) == current_block_num)
		{
/*			for(i = 0; i < tftp_len - 4; i++)
			{
				*(tftp_down_addr + i) = *(tftp_ptr->data + i);
			}
*/
			tftp_send_ack(ADJUST_ENDIAN(tftp_ptr->blocknum), port);
			current_block_num++;
		}
	}
}

MINI2440_STATUS tftp_test(int argc, char(*argv)[MAX_COMMAND_LENGTH])
{
	MINI2440_STATUS status = MINI2440_SUCCESS;
	u32 send_times = 500;
	u32 isr_status = 0;
	
	dm9000_init();

	delay(500);
	
	tftp_send_request("zImage");
	
	delay(100);
	
	while(send_times--)
	{
		delay(500000);
		printf_string(".");
	}
	
	dm9000_int_clear();
	
	return status;
}