#include <stdio.h>
#include <malloc.h>

#include "LinkList.h"
#include "LinkQueue.h"

typedef struct _tag_LinkQueueNode
{
	LinkListNode header;
	void *item;
} LinkQueueNode;

LinkQueue* LinkQueue_Create()
{
	return LinkList_Create();
}

void LinkQueue_Destroy(LinkQueue* linkQueue)
{
	if(linkQueue != NULL)
	{
		LinkQueue_Clear(linkQueue);
		LinkList_Destroy(linkQueue);
	}
}

void LinkQueue_Clear(LinkQueue* linkQueue)
{
	LinkListNode* node = NULL;
	
	if(linkQueue != NULL && LinkList_Length(linkQueue) > 0)
	{
		while(LinkList_Length(linkQueue) > 0)
		{
			node = LinkList_Delete(linkQueue, 0);
			if(node != NULL)
			{
				free(node);
			}
		}
	}
}

int LinkQueue_Append(LinkQueue* linkQueue, void* item)
{
	int ret = -1;
	LinkListNode* node = NULL;
	
	if(linkQueue != NULL && item != NULL)
	{
		node = malloc(sizeof(LinkQueueNode));
		
		if(node != NULL)
		{
			((LinkQueueNode*)node)->item = item;
			
			ret = LinkList_Insert(linkQueue, node, LinkList_Length(linkQueue));
			
			if(!ret)
			{
				free(node);
			}
		}
	}
	
	return ret;
}

void* LinkQueue_Retrieve(LinkQueue* linkQueue)
{
	void* ret = NULL;
	LinkListNode* node = NULL;
	
	if(linkQueue != NULL)
	{
		node = LinkList_Delete(linkQueue, 0);
		if(node != NULL)
		{
			ret = ((LinkQueueNode*)node)->item;
			
			free(node);
		}
	}
	
	return ret;
}

void* LinkQueue_Header(LinkQueue* linkQueue)
{
	void* ret = NULL;
	LinkListNode* node = NULL;
	
	if(linkQueue != NULL)
	{
		node = LinkList_Get(linkQueue, 0);
		if(node != NULL)
		{
			ret = ((LinkQueueNode*)node)->item;
		}
	}
	
	return ret;
}

int LinkQueue_Length(LinkQueue* linkQueue)
{
	return LinkList_Length(linkQueue);
}