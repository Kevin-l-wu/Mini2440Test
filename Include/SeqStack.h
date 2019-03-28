#ifndef _SEQSTACK_H_
#define _SEQSTACK_H_

typedef void SeqStack;

SeqStack* SeqStack_Create(int capacity);

void SeqStack_Destroy(SeqStack* seqStack);

void SeqStack_Clear(SeqStack* seqStack);

int SeqStack_Push(SeqStack* seqStack, void* item);

void* SeqStack_Pop(SeqStack* seqStack);

void* SeqStack_Top(SeqStack* seqStack);

int SeqStack_Size(SeqStack* seqStack);

#endif
