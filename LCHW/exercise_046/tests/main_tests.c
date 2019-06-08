#include <lcthw/dbg.h>
#include <lcthw/list.h>
#include "minunit.h"

List *list;

char *test_list_create(){
    list = List_create();
    mu_assert(list != NULL, "Failed to create list");
    return NULL;
}

char *test_list_push(){
    List_push(list, NULL);
    debug("current list length %d", List_count(list));
    return NULL;
}

char *test_list_pop(){
    List_pop(list);
    debug("current list length %d", List_count(list));    
    return NULL;
}

char *test_list_unshift(){
    List_unshift(list, NULL);
    debug("current list length %d", List_count(list));
    return NULL;
}

char *test_list_shift(){
    List_shift(list);
    debug("current list length %d", List_count(list));    
    return NULL;
}


char *test_list_clear(){
    List_clear(list);
    return NULL;    
}

char *test_list_destroy(){
    List_destroy(list);
    return NULL;
}

char *test_list(){
    mu_suite_start();

    mu_run_test(test_list_create);
    mu_run_test(test_list_push);
    mu_run_test(test_list_unshift);
    mu_run_test(test_list_shift);    
    mu_run_test(test_list_pop);
    mu_run_test(test_list_clear);
    mu_run_test(test_list_destroy);    
    return NULL;
}


RUN_TESTS(test_list)
