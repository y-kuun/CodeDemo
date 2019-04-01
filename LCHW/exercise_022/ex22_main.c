#include "ex22.h"
#include "dbg.h"

const char *MY_NAME = "Yukun Du";

void scope_demo(int count){
    log_info("count is %d", count); 
    
    if(count > 10){
        int count = 100; 
        log_info("count in this scope is %d", count);
    }

    log_info("count is at exit: %d", count);
    
    count = 3000;

    log_info("count after assign: %d", count);
}



int main(int argc, char* argv[])
{
    log_info("My name: %s, age %d", MY_NAME, get_age());
    
    set_age(100);

    log_info("My age is now: %d", get_age());

    log_info("THE_SIZE is: %d", THE_SIZE);
    print_size();

    THE_SIZE = 9;

    log_info("THE_SIZE is now: %d", THE_SIZE);
    print_size();

    log_info("Ratio at first: %f", update_ratio(2.0));
    log_info("Ratio again: %f", update_ratio(10.0));
    log_info("Ratio one more: %f", update_ratio(300.0));

    int count = 4;
    scope_demo(count);
    scope_demo(count * 20);
    
    log_info("count after calling scope_demo: %d", count);

    int a = 10, b = 20;
    log_info("value a: %d value b: %d", a, b);
    fake_swap(a, b);
    log_info("after fake_swap: value a: %d value b: %d", a, b);
    real_swap(&a, &b);
    log_info("after real_swap: value a: %d value b: %d", a, b);
    int *intp = return_stack_value();
    log_info("value on stack intp: %p value: %d", intp, *intp);
 
    /*int blob[1024 * 1024];*/
    /*main(argc, argv);*/
    return 0;

}
