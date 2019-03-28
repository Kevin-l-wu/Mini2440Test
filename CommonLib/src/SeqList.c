#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "SeqList.h"

typedef struct _tag_TSeqList TSeqList;
struct _tag_TSeqList
{
	int capacity;
	int length;
	SeqListNode* node;
};

SeqList* SeqList_Create(int capacity)
{
	TSeqList* ret = NULL;
	
	if(capacity >= 0)
	{
		ret = (SeqList*)malloc(sizeof(TSeqList) + sizeof(SeqListNode) * capacity);
	}
	
	//Init
	if(ret != NULL)
	{
		ret->capacity = capacity;
		ret->length = 0;
		ret->node = (SeqListNode*)(ret + 1);
		
		memset(ret->node, 0, sizeof(SeqListNode) * capacity);
	}
	
	return ret;
};

void SeqList_Destroy(SeqList* seqList)
{
	if(seqList != NULL)
	{
		free(seqList);
	}
}

void SeqList_Clear(SeqList* seqList)
{
	TSeqList* tSeqList = (TSeqList*)seqList;
	
	if(tSeqList != NULL)
	{
		tSeqList->length = 0;
		tSeqList->node = NULL;
	}
}
//-1: Insert fail;  0 Insert Success; 1: Insert fail(Full)
int SeqList_Insert(SeqList* seqList, SeqListNode* node, int pos)
{
	TSeqList* tSeqList = (TSeqList*)seqList;
	int ret = -1;
	int i = 0;

	if((tSeqList != NULL) && (node != NULL))
	{
		if(pos > tSeqList->length)
		{
			pos = tSeqList->length;
		}
		
		if((tSeqList->length < tSeqList->capacity) && (pos >= 0))
		{
			//Move the node after pos
			for(i = tSeqList->length; i > pos; i--)
			{
				tSeqList->node[i] = tSeqList->node[i-1];
			}
			
			tSeqList->node[pos] = (SeqListNode)node;
			
			tSeqList->length = tSeqList->length  + 1;
			
			ret = 0;
		}
		else
		{
			ret = 1;
		}
		
	}
	
	return ret;
}

SeqListNode* SeqList_Delete(SeqList* seqList, int pos)
{
	TSeqList* tSeqList = (TSeqList*)seqList;
	SeqListNode* ret = NULL;
	int i = 0;
	
	if((tSeqList != NULL) && (pos >= 0) && (pos < tSeqList->length))
	{
		ret = (SeqListNode*)(tSeqList->node[pos]);
		
		for(i = pos; i < tSeqList->length; i++)
		{
			tSeqList->node[i] = tSeqList->node[i+1];
		}
		
		tSeqList->node[tSeqList->length] = (SeqListNode)0;
		
		tSeqList->length = tSeqList->length - 1;
	}
	
	return ret;
}

SeqListNode* SeqList_Get(SeqList* seqList, int pos)
{
	TSeqList* tSeqList = (TSeqList*)seqList;
	SeqListNode* ret = NULL;
	
	if((tSeqList != NULL) && (pos >= 0) && (pos < tSeqList->length))
	{
		ret = (SeqListNode*)(tSeqList->node[pos]);
	}
	
	return ret;
}

int SeqList_Length(SeqList* seqList)
{
	TSeqList* tSeqList = (TSeqList*)seqList;
	
	return tSeqList->length;
}

int SeqList_Capacity(SeqList* seqList)
{
	TSeqList* tSeqList = (TSeqList*)seqList;
	
	return tSeqList->capacity;
}
