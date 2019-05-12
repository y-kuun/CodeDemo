#include <lcthw/darray.h>

DArray *DArray_create(size_t element_size, size_t initial_max)
{
    DArray *array = calloc(1, sizeof(DArray));
    check_mem(array);
    array->max = initial_max;
    check(initial_max > 0, "You must set an initial_max > 0");
    
    array->contents = calloc(1, sizeof(void*) * initial_max);
    check_mem(array->contents);
    array->end = 0;
    array->element_size = element_size;
    array->expand_rate = DEFAULT_EXPAND_RATE;
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

static inline int DArray_resize(DArray *array, size_t newsize)
{
    array->max = newsize;
    check(array->max > 0, "The newsize must be > 0.");
    // debug("resize %d ", newsize);
    void *contents = realloc(array->contents, array->max * sizeof(void*));
    check_mem(contents);

    array->contents = contents;
    return 0;
 error:
    return -1;
}

int DArray_expand(DArray *array)
{
    size_t old_max = array->max;
    check(DArray_resize(array, array->max + array->expand_rate) == 0,
          "Failed to expand array to new size: %d",
          array->max + (int)array->expand_rate);
    // debug("expand to %d", array->max);
    memset(array->contents + old_max, 0, array->expand_rate + 1);
    return 0;
error:
    if(array->contents) free(array->contents);
    array->contents = NULL;
    return -1;
}

int DArray_contract(DArray *array)
{
    int new_size = array->end < (int)array->expand_rate ? (int)array->expand_rate : array->end;
    return DArray_resize(array, new_size + 1);
}

void DArray_clear_destroy(DArray *array)
{
    DArray_clear(array);
    DArray_destroy(array);
}

int DArray_push(DArray *array, void *el)
{
    array->contents[array->end] = el;
    array->end++;

    if(DArray_end(array) >= DArray_max(array))
    {
        return DArray_expand(array);
    }
    else
    {
        return 0;
    }
}

void *DArray_pop(DArray *array)
{
    check(array->end - 1 >= 0, "Attemp to pop from empty array.");
    void *el = DArray_remove(array, array->end - 1);
    array->end--;

    if(DArray_end(array) > (int)array->expand_rate && DArray_end(array) % array->expand_rate)
    {
        DArray_contract(array);
    }
    return el;
 error:
    return NULL;
}
