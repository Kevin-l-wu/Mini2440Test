#ifndef _LINKQUEUE_H_
#define _LINKQUEUE_H_

typedef void LinkQueue;

LinkQueue* LinkQueue_Create();

void LinkQueue_Destroy(LinkQueue* linkQueue);

void LinkQueue_Clear(LinkQueue* linkQueue);

int LinkQueue_Append(LinkQueue* linkQueue, void* item);

void* LinkQueue_Retrieve(LinkQueue* linkQueue);

void* LinkQueue_Header(LinkQueue* linkQueue);

int LinkQueue_Length(LinkQueue* linkQueue);

#endif