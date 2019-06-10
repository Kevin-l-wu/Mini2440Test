#ifndef BOOTM_H_
#define BOOTM_H_

#define ATAG_CORE	0x54410001
#define ATAG_MEM	0x54410002
#define ATAG_CMDLINE	0x54410009
#define ATAG_NONE	0x00000000

//#define SDRAM_KERNEL_START (0x30004000 + 0x40)
#define SDRAM_TAG_START 0x30000100

#define SDRAM_ADDR_START   0x30000000
#define SDRAM_TOTAL_SIZE   0x04000000

#define tag_size(type)	((sizeof(TAG_HERDER) + sizeof(type)) >> 2)
#define tag_next(t)	((TAG *)((unsigned int *)(t) + (t)->hdr.size))


typedef struct _TAG_HEADER
{
	unsigned int size;
	unsigned int tag;
} TAG_HERDER;

typedef struct _TAG_CORE
{
	unsigned int flags;
	unsigned int pagesize;
	unsigned int rootdev;
} TAG_CORE;

typedef struct _TAG_MEM32
{
	unsigned int size;
	unsigned int start;
} TAG_MEM32;

typedef struct _TAG_CMDLINE
{
	char cmdline[1];
} TAG_CMDLINE;

typedef struct _TAG
{
	TAG_HERDER hdr;
	
	union {
		TAG_CORE core;
		TAG_MEM32 mem;
		TAG_CMDLINE cmdline;
	} u;
} TAG;


MINI2440_STATUS bootm(int argc, char(*argv)[MAX_COMMAND_LENGTH]);

#endif 