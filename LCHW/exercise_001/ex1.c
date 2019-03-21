#include <stdio.h>

/* This is a comment. */
int main(int argc, char* argv[])
{
    int distance = 100;

    // this is also comment
    printf("You are %d miles away.\n", distance);
    printf("show addr. of distance in %%d %d\n", &distance);
    printf("show addr. of distance in %%p %p\n", &distance);
    printf("show addr. of fucntion main in %%p %p\n", main);
    return 0;
}
