#ifndef __STACK_H__
#define __STACK_H__

#include <lcthw/list.h>

typedef struct{
    List *_st;
} Stack;

Stack *Stack_create();
void Stack_destroy();

void Stack_push(Stack *stack, void* value);
void *Stack_pop(Stack *stack);

void *Stack_peek(Stack *stack);
int Stack_count(Stack *stack);

#define STACK_FOREACH(stack, cur) ListNode *cur = NULL;\
    for(cur = stack->_st->first; cur != stack->_st->last; cur = cur->next)


#endif
