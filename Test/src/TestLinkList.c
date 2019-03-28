#include <stdio.h>
#include "Uart.h"
#include "LinkList.h"
#include "TestLinkList.h"


typedef struct _COMMAND_LIST_NODE
{
	LinkListNode* next;
	int command;
} COMMAND_LIST_NODE;


void TestLinkList()
{
	void* commandLink = LinkList_Create();
	
	COMMAND_LIST_NODE note1;
	note1.command = 23;
	
	
	COMMAND_LIST_NODE *noteGet1 = NULL;
	

	LinkList_Insert(commandLink, (LinkListNode*)&note1, LinkList_Length(commandLink));

	noteGet1 = (COMMAND_LIST_NODE*)LinkList_Get(commandLink, 0);
	
	printf_string("noteGet1->command = %x\n\n", noteGet1->command);
	
	LinkList_Clear(commandLink);
	LinkList_Destroy(commandLink);
}


