#ifndef DM9000PROTOCOL_H_
#define DM9000PROTOCOL_H_

//typedef void (*)();
typedef int (*DM9000INIT)();
typedef void (*DM9000SEND)(unsigned char* data, unsigned int length);
typedef int (*DM9000RECV)();
typedef void (*DM9000INTCLEAR)();
typedef void (*DM9000DEINIT)();
typedef unsigned char(* DM9000CHECKINIT)();

typedef struct _DM9000ModOps
{
	DM9000INIT DM9000Init;
	DM9000DEINIT DM9000DeInit;
	DM9000CHECKINIT DM9000CheckInit;
	DM9000SEND DM9000Send;
	DM9000RECV DM9000Recv;
} DM9000ModOps;


#endif