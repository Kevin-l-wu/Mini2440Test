#ifndef NET_H_
#define NET_H_

#define PROTO_ARP 0x0806
#define PROTO_IP 0x0800
#define PROTO_UDP 0x11

#define ADJUST_ENDIAN(val) (((val) & 0x00ff) << 8 | ((val) & 0xff00) >> 8) 

typedef struct _ETH_HDR
{
	u8 d_mac[6];
	u8 s_mac[6];
	u16 type;
} ETH_HDR;

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

//#define NET_DEBUG

#ifdef NET_DEBUG
#define NET_DBG(fmt, args...) printf_string(fmt, ##args)
#else
#define NET_DBG(fmt,args...)
#endif

#endif