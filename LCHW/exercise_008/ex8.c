#include <stdio.h>

/*char full_name[] = {*/
    /*'Y', 'u', 'k', 'u', 'n',*/
    /*'D', 'u', */
/*};*/

int main(int argc, char* argv[])
{
    int areas[] = {10, 12, 13, 14};
    char name[] = { 'Y', 'u', 'k', 'u', 'n'};
    // 即使删除了字符串终止符，还是可以在其后的内存地址上面找到, 不管是在栈上，还是BBS段上数据段
    char full_name[] = {
        'Y', 'u', 'k', 'u', 'n',
        'D', 'u', 
    };

    printf("The size of an int: %ld.\n", sizeof(int));
    printf("The size of areas (int[]): %ld.\n", sizeof(areas));
    printf("The number of int in areas: %ld.\n", sizeof(areas) / sizeof(int));
    printf("The first area is %d and second is %d.\n", areas[0], areas[10]);

    printf("The size of a char: %ld.\n", sizeof(char));
    printf("The size of name (char[]): %ld.\n", sizeof(name));
    printf("The number of chars in name: %ld.\n", sizeof(name) / sizeof(char));
    
    printf("The size of full_name (char[]): %ld.\n", sizeof(full_name));
    printf("The number of chars in full_name: %ld.\n", sizeof(full_name) / sizeof(char));
    
    printf("name=\"%s\" and full_name=\"%s\"\n", name, full_name);
    return 0;
}
