#ifndef ARP_H_
#define ARP_H_

#define PROTO_ARP 0x0806
#define PROTO_IP 0x0800
#define PROTO_UDP 0x11

typedef struct _ETH_HDR
{
	u8 d_mac[6];
	u8 s_mac[6];
	u16 type;
} ETH_HDR;

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

typedef struct _IP_HDR
{
	ETH_HDR eth_hdr;
	u8 vhl;
	u8 tos;
	u16 iplen;
	u16 ipid;
	u16 ipoffset;
	u8 ttl;
	u8 proto;
	u16 ipchksum;
	u8 src_ip_addr[4];
	u8 dst_ip_addr[4];
} IP_HDR;

typedef struct _UDP_HDR
{
	IP_HDR ip_hdr;
	u16 sport;
	u16 dport;
	u16 udplen;
	u16 udpchksum;
} UDP_HDR;

typedef struct _TFTP_PACKAGE
{
	u16 opcode;
	u16 blocknum;
	u8 data[0];
} TFTP_PACKAGE;


MINI2440_STATUS arp_test(int argc, char(*argv)[MAX_COMMAND_LENGTH]);

#endif