#include <stdio.h>
#include "Uart.h"
#include "CoreMalloc.h"

MALLOC gMallocPool[MALLOC_MAX_SIZE] = {0};

void* CoreMalloc(unsigned length)
{
	void* ret = NULL;
	unsigned i = 0;
	
	if(length > MALLOC_POOL_SIZE)
	{
		printf("Malloc length > MALLOC_POOL_SIZE, malloc fail\n");
		return ret;
	}
	
	for(i = 0; i < MALLOC_MAX_SIZE; i++)
	{
		printf("Malloc: i = 0x%x\n", i);
		if(gMallocPool[i].isFreed == 0)
		{
			gMallocPool[i].isFreed = 1;
			ret = gMallocPool[i].mallocPool;
			break;
		}
	}
	
//	printf("malloc(): ret = %x\n", ret);
	
	return ret;
}

void CoreFree(void* ptr)
{
	int i = 0;
	
	if(ptr != NULL)
	{
		for(i = 0; i < MALLOC_MAX_SIZE; i++)
		{
			if(gMallocPool[i].mallocPool == ptr)
			{
				gMallocPool[i].isFreed = 0;
				printf("Free gMallocPoo[%d]\n", i);
				break;
			}
		}
	}
	else
	{
		printf("Freed failed, NULL pointer input\n");
	}
}
