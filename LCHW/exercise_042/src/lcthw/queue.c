#include <lcthw/queue.h>
#include <lcthw/dbg.h>

Queue *Queue_create()
{
    Queue* queue = calloc(1, sizeof(Queue));
    check_mem(queue);
    queue->_q = List_create();
    check_mem(queue->_q);
    return queue;
error:
    Queue_destroy(queue);
    return NULL;
}

void Queue_destroy(Queue *queue)
{
    if(queue)
    {
        List_destroy(queue->_q);
        free(queue);
    }
    
}

void Queue_send(Queue *queue, void* value)
{
    List_push(queue->_q, value);
}

void *Queue_recv(Queue *queue)
{
    return List_shift(queue->_q);
}

void *Queue_peek(Queue *queue)
{
    return List_first(queue->_q);
}

int Queue_count(Queue *queue)
{
    return List_count(queue->_q);
}
