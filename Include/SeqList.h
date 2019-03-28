#ifndef _SEQLIST_H_
#define _SEQLIST_H_

typedef void SeqList;
typedef unsigned int SeqListNode;

SeqList* SeqList_Create(int capacity);

void SeqList_Destroy(SeqList* seqList);

void SeqList_Clear(SeqList* seqList);

int SeqList_Insert(SeqList* seqList, SeqListNode* node, int pos);

SeqListNode* SeqList_Delete(SeqList* seqList, int pos);

SeqListNode* SeqList_Get(SeqList* seqList, int pos);

int SeqList_Length(SeqList* seqList);

int SeqList_Capacity(SeqList* seqList);

#endif
