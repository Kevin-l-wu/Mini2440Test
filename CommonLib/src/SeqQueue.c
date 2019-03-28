#include <stdio.h>

#include "SeqList.h"
#include "SeqQueue.h"

SeqQueue* SeqQueue_Create(int capacity)
{
	return SeqList_Create(capacity);
}

void SeqQueue_Destroy(SeqQueue* seqQueue)
{
	return SeqList_Destroy(seqQueue);
}

void SeqQueue_Clear(SeqQueue* seqQueue)
{
	return SeqList_Clear(seqQueue);
}

int SeqQueue_Append(SeqQueue* seqQueue, void* item)
{
	return SeqList_Insert(seqQueue, item, SeqList_Length(seqQueue));
}

void* SeqQueue_Retrieve(SeqQueue* seqQueue)
{
	return SeqList_Delete(seqQueue, 0);
}

void* SeqQueue_Header(SeqQueue* seqQueue)
{
	return SeqList_Get(seqQueue, 0);
}

int SeqQueue_Length(SeqQueue* seqQueue)
{
	return SeqList_Length(seqQueue);
}

int SeqQueue_Capacity(SeqQueue* seqQueue)
{
	return SeqList_Capacity(seqQueue);
}