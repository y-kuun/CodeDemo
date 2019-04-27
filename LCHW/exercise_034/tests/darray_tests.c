#include "minunit.h"
#include <lcthw/darray.h>

static DArray *array = NULL;
// static int *val1 = NULL;
// static int *val2 = NULL;

char *test_create()
{
    array = DArray_create(sizeof(int), 100);
    mu_assert(array != NULL, "DArray_create failed.");
    mu_assert(array->contents != NULL, "contents are wrong in darray");
    mu_assert(array->end == 0, "end isn't at the right spot");
    mu_assert(array->element_size == sizeof(int), "element size is wrong.");
    mu_assert(array->max == 100, "wrong max length on initial size");
    return NULL;
}

char *test_destroy()
{
    DArray_destroy(array);
    return NULL;
}

char *test_new()
{
    return NULL;
}

char *test_set()
{
    return NULL;
}

char *test_get()
{
    return NULL;
}

char *test_remove()
{
    return NULL;
}

char *test_expand_contract()
{
    return NULL;
}

char *test_push_pop()
{
    return NULL;
}

char *all_tests()
{
    mu_suite_start();
    mu_run_test(test_create);
    mu_run_test(test_new);
    mu_run_test(test_set);
    mu_run_test(test_get);
    mu_run_test(test_remove);
    mu_run_test(test_expand_contract);
    mu_run_test(test_push_pop);
    mu_run_test(test_destroy);
    
    return NULL;
}


RUN_TESTS(all_tests);
