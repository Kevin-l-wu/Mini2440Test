#ifndef ARP_H_
#define ARP_H_


typedef struct _ARP_HDR
{
	ETH_HDR eth_hdr;
	u16 hw_type;
	u16 protocol;
	u8 hw_len;
	u8 protocol_len;
	u16 opcode;
	u8 src_mac[6];
	u8 src_ip_addr[4];
	u8 dst_mac[6];
	u8 dst_ip_addr[4];
} ARP_HDR;



#endif