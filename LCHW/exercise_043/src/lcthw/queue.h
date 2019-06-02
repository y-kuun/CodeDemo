#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <lcthw/list.h>

typedef struct{
    List *_q;
} Queue;

Queue *Queue_create();
void Queue_destroy(Queue *queue);

void Queue_send(Queue *queue, void* value);
void *Queue_recv(Queue *queue);

void *Queue_peek(Queue *queue);
int Queue_count(Queue *queue);

#define QUEUE_FOREACH(queue, cur) ListNode *cur = NULL;\
    for(cur = queue->_q->first; cur != queue->_q->last; cur = cur->next)


#endif
