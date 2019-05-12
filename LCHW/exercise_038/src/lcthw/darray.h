#ifndef __DARRAY_H__
#define __DARRAY_H__

#include <stdlib.h>
#include <assert.h>
#include <lcthw/dbg.h>

#define DEFAULT_EXPAND_RATE 300

typedef struct DArray
{
    int end;
    int max;
    size_t element_size;
    size_t expand_rate;
    void **contents;
} DArray;

DArray *DArray_create(size_t element_size, size_t initial_max);

void DArray_destroy(DArray *array);

void DArray_clear(DArray *array);

int DArray_expand(DArray *array);

int DArray_contract(DArray *array);

int DArray_push(DArray *array, void *el);

void *DArray_pop(DArray *array);

void DArray_clear_destroy(DArray *array);

#define DArray_last(A) ((A)->contents[(A)->end - 1])
#define DArray_first(A) ((A)->contents[0])
#define DArray_end(A) ((A)->end)
#define DArray_count(A) DArray_end(A)
#define DArray_max(A) ((A)->max)

static inline void DArray_set(DArray *array, int i, void *el)
{
    check(i < array->max, "DArray attempt to set past max");
    if(i > array->end) array->end = i;
    array->contents[i] = el;
error:
    return;
}

static inline void *DArray_get(DArray *array, int i)
{
    check(i < array->max, "DArray attempt to get past max");
    return array->contents[i];
error:
    return NULL;
}

static inline void *DArray_remove(DArray *array, int i)
{
    check(i < array->max, "DArray attempt to remove past max");
    void *el = array->contents[i];
    array->contents[i] = NULL;
    return el;
error:
    return NULL;
}

static inline void *DArray_new(DArray *array)
{
    // allocate a new memory area with element size
    check(array->element_size > 0, "Can't use DArray_new on 0 size DArrays.");
    return calloc(1, array->element_size);
error:
    return NULL;
}

static inline void *DArray_free(void *ele)
{
    if(ele)
    {
        free(ele);
    }
    return NULL;
}

#endif
