#ifndef __DARRAY_ALGOS_H__
#define __DARRAY_ALGOS_H__

#include <lcthw/darray.h>
// #include <stdlib.h>

typedef int (*DArray_compare)(const void*, const void*);

int DArray_qsort(DArray *array, DArray_compare cmp);

int DArray_heapsort(DArray *array, DArray_compare cmp);

int DArray_mergesort(DArray *array, DArray_compare cmp);

void** DArray_find(DArray *array, DArray_compare cmp, void *el);

static inline int DArray_sort_add(DArray *array, int (*func)(DArray*, DArray_compare), DArray_compare cmp, void *el)
{
    int rc = 0;
    rc = DArray_push(array, el);
    check(rc == 0, "Failed to push new element");
    rc = func(array, cmp);
    check(rc == 0, "Failed to sort!!");
    return 0;
error:
    return -1;
}

static inline void print_array(void **data, int ridx)
{
    int i = 0;
    for(i = 0; i <= ridx; i ++){
        char *str = data[i];
        printf("%s\t", str);
    }
    printf("\n");
}

#endif
