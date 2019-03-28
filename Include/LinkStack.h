#ifndef _LINKSTACK_H_
#define _LINKSTACK_H_

typedef void LinkStack;

LinkStack* LinkStack_Create();

void LinkStack_Destroy(LinkStack* linkStack);

void LinkStack_Clear(LinkStack* linkStack);

int LinkStack_Push(LinkStack* linkStack, void* item);

void* LinkStack_Pop(LinkStack* linkStack);

void* LinkStack_Top(LinkStack* linkStack);

int LinkStack_Size(LinkStack* linkStack);

#endif
