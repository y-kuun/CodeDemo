#ifndef __DARRAY_ALGOS_H__
#define __DARRAY_ALGOS_H__

#include <lcthw/darray.h>
#include <stdlib.h>

typedef int (*DArray_compare)(const void*, const void*);

int DArray_qsort(DArray *array, DArray_compare cmp);
#if 0
{
    qsort(array->contents, DArray_count(array), szieof(void*), cmp);
}
#endif

int DArray_heapsort(DArray *array, DArray_compare cmp);
#if 0
{
    return heapsort(array->contents, DArray_count(array), szieof(void*), cmp);
}
#endif

int DArray_mergesort(DArray *array, DArray_compare cmp);
#if 0
{
    return mergesort(array->contents, DArray_count(array), szieof(void*), cmp);
}
#endif

#endif
