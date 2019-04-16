#include "minunit.h"
#include <dlfcn.h>

typedef int (*lib_function)(const char *data);
typedef int (*lib_function_safe)(const char *data, const int cnt);
char *lib_file = "./libex29.so";
void *lib = NULL;

char *test_dlopen() {
    lib = dlopen(lib_file, RTLD_NOW);
    mu_assert(lib != NULL, "Failed to open the library to test.");
    return NULL;
}

int check_function(const char *func_to_run, const char *data, int expt){
    lib_function func = dlsym(lib, func_to_run);
    check(func != NULL, "Did not find %s function in library %s: %s", func_to_run, lib_file, dlerror());

    int rc = func(data);
    check(rc == expt, "Function %s return %d for data: %s", func_to_run, rc, data);

    return 1;
error:
    return 0;
}

int check_function_safe(const char *func_to_run, const char *data, int n, int expt){
    lib_function_safe func = dlsym(lib, func_to_run);
    check(func != NULL, "Did not find %s function in library %s: %s", func_to_run, lib_file, dlerror());

    int rc = func(data, n);
    check(rc == expt, "Function %s return %d for data: %s", func_to_run, rc, data);

    return 1;
error:
    return 0;
}


char *test_functions() {
    mu_assert(check_function("print_a_message", "Hello", 0), "print_a_message failed.");
    mu_assert(check_function("uppercase", "Hello", 0), "uppercase failed.");
    mu_assert(check_function("lowercase", "Hello", 0), "lowercase failed.");
    
    mu_assert(check_function_safe("print_a_message_safe", "Hello", 5, 0), "print_a_message_safe failed.");
    mu_assert(check_function_safe("uppercase_safe", "Hello", 5,  0), "uppercase_safe failed.");
    mu_assert(check_function_safe("lowercase_safe", "Hello", 5, 0), "lowercase_safe failed.");
    
    return NULL;
}

char *test_failures() {
    mu_assert(check_function("fail_on_purpose", "Hello", 1), "fail_on_purpose should fail.");
    return NULL;
}

char *test_dlclose() {
    int rc = dlclose(lib);
    mu_assert(rc == 0, "Failed to close lib.");
    return NULL;
}

char *all_tests() {
    mu_suite_start();

    mu_run_test(test_dlopen);
    mu_run_test(test_functions);
    mu_run_test(test_failures);
    mu_run_test(test_dlclose);

    return NULL;
}

RUN_TESTS(all_tests);
