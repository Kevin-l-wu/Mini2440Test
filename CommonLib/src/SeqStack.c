#include <stdio.h>

#include "SeqStack.h"
#include "SeqList.h"

SeqStack* SeqStack_Create(int capacity)
{
	return SeqList_Create(capacity);
}

void SeqStack_Destroy(SeqStack* seqStack)
{
	SeqList_Destroy(seqStack);
}

void SeqStack_Clear(SeqStack* seqStack)
{
	SeqList_Clear(seqStack);
}

int SeqStack_Push(SeqStack* seqStack, void* item)
{
	SeqList_Insert(seqStack, item, SeqList_Length(seqStack));
}

void* SeqStack_Pop(SeqStack* seqStack)
{
	return SeqList_Delete(seqStack, SeqList_Length(seqStack));
}

void* SeqStack_Top(SeqStack* seqStack)
{
	return SeqList_Get(seqStack, SeqList_Length(seqStack));
}

int SeqStack_Size(SeqStack* seqStack)
{
	return SeqList_Length(seqStack);
}


int SeqStack_Capacity(SeqStack* seqStack)
{
	return SeqList_Capacity(seqStack);
}