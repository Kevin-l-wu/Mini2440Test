#ifndef _SEQQUEUE_H_
#define _SEQQUEUE_H_

typedef void SeqQueue;

SeqQueue* SeqQueue_Create(int capacity);

void SeqQueue_Destroy(SeqQueue* seqQueue);

void SeqQueue_Clear(SeqQueue* seqQueue);

int SeqQueue_Append(SeqQueue* seqQueue, void* item);

void* SeqQueue_Retrieve(SeqQueue* seqQueue);

void* SeqQueue_Header(SeqQueue* seqQueue);

int SeqQueue_Length(SeqQueue* seqQueue);

int SeqQueue_Capacity(SeqQueue* seqQueue);

#endif