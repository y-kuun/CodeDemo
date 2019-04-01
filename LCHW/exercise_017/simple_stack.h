#pragma once
#include <stdint.h>

typedef struct {
    uint32_t MAX_SIZE;
    void *bp;
    void *sp;
} Stack;

static Stack *st = NULL;

void init_stack(const uint32_t max_size);
void destroy_stack();

void stack_push(void *, const uint32_t size);
void stack_pop(void *, const uint32_t size);

uint32_t stack_size();
void stack_print();

