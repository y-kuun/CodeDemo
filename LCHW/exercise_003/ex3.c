#include <stdio.h>

int main(int argc, char* argv[])
{
    int age = 49;                // variable on stack, directly USE results to Undefined Behavior, 0 on Debian XX on win32
    int height = 75;
    
    printf("I am %d years old.\n", age);
    printf("I am %d inches tall.\n", height);
    
    // test begins
    printf("I am %o years old.\n", age);
    printf("I am %#o years o\bld.\n", age);
    printf("I am \b\b\b%x years old.\n", age);
    printf("I am |%1d| years old.\n", age);
    printf("I am |%-3d| years old.\n", age);
    printf("I am |%3d| years old.\n", age);
    printf("I am %f years old.\n", age);
    printf("I am %c years old.\n", age);
    /*printf("I am %s years old.\n", age);*/
    /* 
        I am 49 years old.
        I am 75 inches tall.
        I am 61 years old.
        I am 061 years ld.
        I 31 years old.
        I am |49| years old.
        I am |49 | years old.
        I am | 49| years old.
        I am 0.000000 years old.
        I am 1 years old.
     */

    return 0;
}
