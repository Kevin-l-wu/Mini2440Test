#include <stdio.h>
#include "GlobalDefine.h"
#include "String.h"
#include "Error.h"
#include "Bootm.h"

TAG* cur_tag_ptr = NULL;
void (*the_kernel)(int, int, unsigned int);

const char *cmdline = "console=ttySAC0,115200 init=/init";

void setup_core_tag()
{
	cur_tag_ptr = (TAG*)SDRAM_TAG_START;

	cur_tag_ptr->hdr.tag = ATAG_CORE;
	cur_tag_ptr->hdr.size = tag_size(TAG_CORE);

	cur_tag_ptr->u.core.flags = 0;
	cur_tag_ptr->u.core.pagesize = 4096;
	cur_tag_ptr->u.core.rootdev = 0;

	cur_tag_ptr = tag_next(cur_tag_ptr);
}

void setup_memory_tag()
{
	cur_tag_ptr->hdr.tag = ATAG_MEM;
	cur_tag_ptr->hdr.size = tag_size(TAG_MEM32);
	
	cur_tag_ptr->u.mem.size = SDRAM_TOTAL_SIZE;
	cur_tag_ptr->u.mem.start = SDRAM_ADDR_START;
	
	cur_tag_ptr = tag_next(cur_tag_ptr);
}

void setup_cmdline_tag()
{
	int cmdline_len = strlen((char*)cmdline);
	
	cur_tag_ptr->hdr.tag = ATAG_CMDLINE;
	cur_tag_ptr->hdr.size = (sizeof(TAG_HERDER) + cmdline_len + 1 + 4) >> 2;
	
	strcpy(cur_tag_ptr->u.cmdline.cmdline, (char*)cmdline);
	
	cur_tag_ptr = tag_next(cur_tag_ptr);
}

void setup_end_tag()
{
	cur_tag_ptr->hdr.tag = ATAG_NONE;
	cur_tag_ptr->hdr.size = 0;	
}

void boot_linux()
{
	/* 1. Get Linux boot address */
	the_kernel = (void (*)(int, int, unsigned int))SDRAM_KERNEL_START;
	
	/* 2. Set boot parameter */
	
	/* 2.1 Set core parameter */
	setup_core_tag();
	
	/* 2.1 Set memory parameter */
	setup_memory_tag();
	
	/* 2.1 Set command line parameter */
	setup_cmdline_tag();

	/* 2.1 Set parameter end flag */
	setup_end_tag();	
	
	/* 3. Start boot */
	the_kernel(0, 1999, SDRAM_TAG_START);
}

MINI2440_STATUS bootm(int argc, char(*argv)[MAX_COMMAND_LENGTH])
{
	MINI2440_STATUS status = MINI2440_SUCCESS;
	
	boot_linux();
	
	return status;
}