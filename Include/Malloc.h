#ifndef MALLOC_H_
#define MALLOC_H_


#define MALLOC_MAX_SIZE  100
#define MALLOC_POOL_SIZE 100

typedef struct _MALLOC
{
	char isFreed;
	char mallocPool[MALLOC_POOL_SIZE];
} MALLOC;


#endif