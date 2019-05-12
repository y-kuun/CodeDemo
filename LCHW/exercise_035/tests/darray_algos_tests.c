#include "minunit.h"
#include <lcthw/darray_algos.h>

char *new_add = "adasdasdadgdf";

int testcmp(char **a, char **b)
{
    return strcmp(*a, *b);
}

DArray *create_words()
{
    DArray *result = DArray_create(0, 1);
    char *words[] = {"asdfasfd", "werwar", "13234", "asdfasfd", "oioj", "ue7823", "000123", "-123123", "safhjjks", "jsdfghkjkjshd", "83274rhjskndb7382tweyuidhj"};
    int i = 0;
    for(i = 0; i < 11; i++)
    {
        DArray_push(result, words[i]);
    }
    return result;
}

int is_sorted(DArray *array)
{
    int i = 0;
    for(i = 0; i < DArray_count(array) - 1; i++)
    {
        if(strcmp(DArray_get(array, i), DArray_get(array, i + 1)) > 0)
        {
            return 0;
        }
    }
    return 1;
}

char *run_sort_test(int (*func)(DArray*, DArray_compare), const char *name)
{
    DArray *words = create_words();
    mu_assert(!is_sorted(words), "Words should start not sorted.");

    debug("--- Testing %s sorting algorithm", name);
    int rc = func(words, (DArray_compare)testcmp);
    mu_assert(rc == 0, "sort failed");
    mu_assert(is_sorted(words), "didn't sort it");
    // DArray_push(words, new_add);
    // rc = func(words, (DArray_compare)testcmp);    
    rc = DArray_sort_add(words, func, (DArray_compare)testcmp, new_add);
    mu_assert(rc == 0, "sort failed");
    mu_assert(is_sorted(words), "didn't sort it");    
    DArray_destroy(words);
    return NULL;
}

char *test_qsort()
{
    return run_sort_test(DArray_qsort, "qsort");
}

char *test_heapsort()
{
    return run_sort_test(DArray_heapsort, "heapsort");
}

char *test_mergesort()
{
    return run_sort_test(DArray_mergesort, "mergesort");
}

char *all_tests()
{
    mu_suite_start();

    mu_run_test(test_qsort);
    mu_run_test(test_heapsort);
    mu_run_test(test_mergesort);
    
    return NULL;
}

RUN_TESTS(all_tests);
