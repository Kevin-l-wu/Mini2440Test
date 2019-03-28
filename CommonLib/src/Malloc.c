#include <stdio.h>
#include "Uart.h"
#include "Malloc.h"

MALLOC gMallocPool[MALLOC_MAX_SIZE] = {0};

void* malloc(int length)
{
	void* ret = NULL;
	unsigned i = 0;
	
	if(length > MALLOC_POOL_SIZE)
	{
		print_string("Malloc length > MALLOC_POOL_SIZE, malloc fail\n");
		return ret;
	}
	
	for(i = 0; i < MALLOC_MAX_SIZE; i++)
	{
		if(gMallocPool[i].isFreed == 0)
		{
			gMallocPool[i].isFreed = 1;
			ret = gMallocPool[i].mallocPool;
			break;
		}
	}
	
//	printf_string("malloc(): ret = %x\n", ret);
	
	return ret;
}

void free(void* ptr)
{
	int i = 0;
	
	if(ptr != NULL)
	{
		for(i = 0; i < MALLOC_MAX_SIZE; i++)
		{
			if(gMallocPool[i].mallocPool == ptr)
			{
				gMallocPool[i].isFreed = 0;
				printf_string("Free gMallocPoo[%d]\n", i);
				break;
			}
		}
	}
	else
	{
		print_string("Freed failed, NULL pointer input\n");
	}
}
