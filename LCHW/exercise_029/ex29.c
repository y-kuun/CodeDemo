#include <stdio.h>
#include "dbg.h"
#include <dlfcn.h>

typedef int (*lib_function)(const char *data);
typedef int (*lib_function_safe)(const char *data, const int cnt);

int main(int argc, char* argv[]){
    int rc = 0;
    check(argc == 5, "USAGE: ex29 libex29.so function data data_len");
    
    char *lib_file = argv[1];
    char *func_to_run = argv[2];
    char *data = argv[3];
    unsigned int n = atoi(argv[4]);

    void *lib = dlopen(lib_file, RTLD_NOW);
    check(lib != NULL, "Failed to open the library %s: %s", lib_file, dlerror());

    // lib_function func = dlsym(lib, func_to_run);
    lib_function_safe func = dlsym(lib, func_to_run);
    check(func != NULL, "Did not find %s function in the library %s: %s", func_to_run, lib_file, dlerror());

    rc = func(data, n);
    check(rc == 0, "Function %s return %d for data: %s", func_to_run, rc, data);

    rc = dlclose(lib);
    check(rc == 0, "Failed to close %s", lib_file);

    return 0;
error:
    return 1;
}
