#include <lcthw/darray.h>

DArray *DArray_create(size_t element_size, size_t initial_max)
{
    DArray *array = calloc(1, sizeof(DArray));
    check_mem(array);
    array->contents = calloc(1, sizeof(void*) * initial_max);
    check_mem(array->contents);
    array->end = 0;
    array->max = initial_max;
    array->element_size = element_size;
    array->expand_rate = 2;
    return array;
error:
    if(array) free(array);
    array = NULL;
    return array;
}

void DArray_destroy(DArray *array)
{
    if(array)
    {
        if(array->contents)
        {
            free(array->contents);
        }
        free(array);        
    }
}

void DArray_clear(DArray *array)
{
    if(array)
    {
        if(array->contents)
        {
            void** data = array->contents;
            int idx = 0;
            for(idx = 0; idx < array->max; idx++)
            {
                if(*data) free(*data);
                *data = NULL;
                data += 1;
            }
        }
        
    }
}

int DArray_expand(DArray *array)
{
    if(array)
    {
        array->max *= array->expand_rate;
        array->contents = realloc(array->contents, array->max * sizeof(void*));
        check_mem(array->contents);
    }
    return 0;
error:
    if(array->contents) free(array->contents);
    array->contents = NULL;
    return -1;
}
