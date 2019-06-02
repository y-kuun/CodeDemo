#include <lcthw/stack.h>
#include <lcthw/dbg.h>

Stack *Stack_create(){
    Stack* stack = calloc(1, sizeof(Stack));
    check_mem(stack);
    stack->_st = List_create();
    check_mem(stack->_st);
    return stack;
error:
    Stack_destroy(stack);
    return NULL;
}
void Stack_destroy(Stack* stack)
{
    if(stack)
    {
        List_destroy(stack->_st);
        free(stack);
    }
    
}

void Stack_push(Stack *stack, void* value)
{
    List_push(stack->_st, value);
}

void *Stack_pop(Stack *stack)
{
    return List_pop(stack->_st);
}

void *Stack_peek(Stack *stack)
{
    return List_last(stack->_st);
}

int Stack_count(Stack *stack)
{
    return List_count(stack->_st);
}
