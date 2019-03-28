#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "simple_stack.h"

void init_stack(const uint32_t max_size) {
    if(st == NULL){
        st = malloc(sizeof(Stack));
        st->MAX_SIZE = max_size;
        st->bp = malloc(sizeof(void) * max_size);
        st->sp = st->bp;
    } else if (max_size > st->MAX_SIZE) {
        uint32_t cur_size = stack_size();
        st->MAX_SIZE = max_size; 
        st->bp = realloc(st->bp, cur_size);
        st->sp = st->bp + cur_size;
    }    
}

void destroy_stack(){
    if(st){
        if(st->bp){
            free(st->bp); 
        } 
        free(st);
        st = NULL;  
    }
}

void stack_push(void *src, const uint32_t size){ 
    assert(st);
    assert(st->sp + size - st->bp <= st->MAX_SIZE);
    memcpy(st->sp, src, size);
    st->sp += size;
}

void stack_pop(void *dst, const uint32_t size){
    assert(st);
    assert(st->sp - size >= st->bp);
    st->sp -= size;
    memcpy(dst, st->sp, size);
}

uint32_t stack_size(){
    assert(st);
    return (uint32_t)(st->sp - st->bp);
}

void stack_print(){
    assert(st);
    printf("Simple Stack Info:\n");
    printf("\tbp:\t%p\n", st->bp);
    printf("\tsp:\t%p\n", st->sp);
    printf("\tvol:%d\n", st->MAX_SIZE);
    printf("\tsize:%d\n", stack_size());
}
