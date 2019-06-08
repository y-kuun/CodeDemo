#include "minunit.h"
#include <lcthw/tstree.h>
#include <lcthw/bstrlib.h>

#include <string.h>
#include <assert.h>

TSTree *node = NULL;
char *valueA = "VALUEA";
char *valueB = "VALUEB";
char *value2 = "VALUE2";
char *value4 = "VALUE4";
char *reverse = "VALUER";
int traverse_count = 0;

struct tagbstring test1 = bsStatic("TEST");
struct tagbstring test2 = bsStatic("TEST2");
struct tagbstring test3 = bsStatic("TSET");
struct tagbstring test4 = bsStatic("T");

char *test_insert()
{
    node = TSTree_insert(node, bdata(&test1), blength(&test1), valueA);
    mu_assert(node != NULL, "Failed to insert into tst.");

    node = TSTree_insert(node ,bdata(&test2), blength(&test2), value2);
    mu_assert(node != NULL, "Failed to insert into tst with second name.");    

    node = TSTree_insert(node ,bdata(&test3), blength(&test3), reverse);
    mu_assert(node != NULL, "Failed to insert into tst with reverse name.");    

    node = TSTree_insert(node ,bdata(&test4), blength(&test4), value4);
    mu_assert(node != NULL, "Failed to insert into tst with second name.");

    return NULL;
}

char *test_search_exact()
{
    void *res = TSTree_search(node, bdata(&test1), blength(&test1));
    mu_assert(res == valueA, "Got the wrong value back, should get A not B.");

    res = TSTree_search(node, "TESTNO", strlen("TESTNO"));
    mu_assert(res == NULL, "Should not find anyhting");

    return NULL;
}
