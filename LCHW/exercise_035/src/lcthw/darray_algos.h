#ifndef __DARRAY_ALGOS_H__
#define __DARRAY_ALGOS_H__

#include <lcthw/darray.h>
// #include <stdlib.h>

typedef int (*DArray_compare)(const void*, const void*);

int DArray_qsort(DArray *array, DArray_compare cmp);

int DArray_heapsort(DArray *array, DArray_compare cmp);

int DArray_mergesort(DArray *array, DArray_compare cmp);

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

#endif
