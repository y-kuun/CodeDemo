#include <stdio.h>

int main(int argc, char* argv[])
{
    int age;                // variable on stack, directly USE results to Undefined Behavior
    int height = 75;
    
    printf("I am %d years old.\n", age);
    printf("I am %d inches tall.\n", height);
    return 0;
}
