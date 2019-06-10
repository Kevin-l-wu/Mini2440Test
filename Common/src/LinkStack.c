#include <stdio.h>
#include <malloc.h>

#include "LinkStack.h"
#include "LinkList.h"

typedef struct _tag_LinkStackNode
{
	LinkListNode header;
	void* item;
} TLinkStackNode;

LinkStack* LinkStack_Create()
{
	return LinkList_Create();
}

void LinkStack_Destroy(LinkStack* linkStack)
{
	LinkStack_Clear(linkStack);
	LinkList_Destroy(linkStack);
}

void LinkStack_Clear(LinkStack* linkStack)
{
	while(LinkStack_Size(linkStack) > 0)
	{
		LinkStack_Pop(linkStack);
	}
}

int LinkStack_Push(LinkStack* linkStack, void* item)
{
	TLinkStackNode* node = (TLinkStackNode*)malloc(sizeof(TLinkStackNode));
	int ret = (node != NULL) && (item != NULL);
	
	if(ret)
	{
		node->item = item;
		ret = LinkList_Insert(linkStack, (LinkListNode*)node, 0);
	}
	
	if( !ret)
	{
		free(node);
	}
	
	return ret;
}

void* LinkStack_Pop(LinkStack* linkStack)
{
	TLinkStackNode* node = (TLinkStackNode*)LinkList_Delete(linkStack, 0);
	void* ret = NULL;
	
	if( node != NULL)
	{
		ret = node->item;
		
		free(node);
	}
	
	return ret;
}

void* LinkStack_Top(LinkStack* linkStack)
{
	TLinkStackNode* node = (TLinkStackNode*)LinkList_Get(linkStack, 0);
	void* ret = NULL;
	
	if( node != NULL)
	{
		ret = node->item;
	}
	
	return ret;
}

int LinkStack_Size(LinkStack* linkStack)
{
	return LinkList_Length(linkStack);
}
