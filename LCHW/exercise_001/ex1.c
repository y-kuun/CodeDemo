#include <stdio.h>

int main(int argc, char* argv[])
{
    puts("Hello World!\n");
    printf("return value of puts: %d\n", puts("Hello\n\tWold!\n"));
    char str0[] = {0x41, 0x31, 0x0};
    putchar(str0[0]); 
    printf("return value of putchar: %d\n", putchar(258u));
    printf("return value of putchar: %f\n", 258.f);
    /*  
    // UB 提前了ex3的内容
        LC6:
            .ascii "return value of putchar: %c %f %d %p %s %d %s %s \12\0"
        .loc 1 12 5
        movl	$97, 4(%esp)
        movl	$LC6, (%esp)
        call	_printf
    printf("return value of putchar: %c %f %d %s %s %p %d %s %s %s %s\n", 0x61);
    */
    putchar(*(str0+1)); 
    putchar(*(str0+2)); 
    putchar('|'); 
    putchar('\n'); 
    int a = 'aa';
    printf("NXPH ~ %d", a);
    /*int b;*/
    /*scanf("read from user %d", &b);*/
    /*printf("read from user: %d", b);*/
    return 0;
}
