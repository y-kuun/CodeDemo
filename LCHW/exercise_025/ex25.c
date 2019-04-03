#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "dbg.h"

#define MAX_DATA 100

int read_string(char **out_string, int max_buffer){
    *out_string = malloc(sizeof(char) * (max_buffer + 1));
    memset(*out_string, 0, sizeof(char) * (max_buffer + 1));
    check_mem(*out_string);
    int i = 0;
    // 这里的翻译可能有问题
    while(i < max_buffer){
        char inp_char = fgetc(stdin);
        check(inp_char != EOF, "input error");
        if(inp_char == '\n' || inp_char == '\0') break; 
        (*out_string)[i] = inp_char;
        i++;
    }
    return 0;
error:
    if(*out_string) free(*out_string);
    *out_string = NULL;
    return -1;
}

int read_int(int *out_int){
    char *input = NULL;
    int rc = read_string(&input, MAX_DATA);
    check(rc == 0, "failed to read number.");

    *out_int = atoi(input);
    free(input);
    return 0;
error:
    if(input) free(input);
    return -1;
}


int put_string(const char* str, int size){
    int i = 0;
    do {
        putchar(str[i]);
        i++;
    }
    while(str[i] != '\0' && i < size);
    return i;
}

void put_int(int num){
    const int max_size = 64;
    static char nums[max_size];
    int i = 0;
    int flag = 0;

    if(num < 0){
        num *= -1;
        flag = 1;
    }
    while(num > 0){
        char ch = '0' + num % 10;
        num = num / 10;
        i ++;
        nums[max_size - i] = ch;
    }
    if(i == 0){
        i++;
        nums[max_size - i] = '0';
    }
    if(flag != 0){
        i++;
        nums[max_size - i] = '-';
    }
    put_string(nums + max_size - i, i);
}


int print_format(const char *fmt, ...){
    int i = 0;
    va_list argp;
    /*int len;*/
    /*char *str;*/

    va_start(argp, fmt);
    for(i = 0; fmt[i] != '\0'; i++){
        if(fmt[i] == '%'){
            i++;
            switch(fmt[i]) {
                case '\0':
                    return 0;
                case 'd':
                    put_int(va_arg(argp, int));
                    break;
                case 'c':
                    putchar(va_arg(argp, char));
                    break;
                case 's': {
                    int len = va_arg(argp, int);
                    char *str = va_arg(argp, char *);
                    put_string(str, len);
                    break;
                }
                default:
                    putchar(fmt[i]);
            }
        } else {
            putchar(fmt[i]);
        } 
    }
    va_end(argp);
    return 0;
}

int read_scan(const char *fmt, ...){
    int i = 0;
    int rc = 0;
    int *out_int = NULL;
    char *out_char = NULL;
    char **out_string = NULL;
    int max_buffer = 0;
    
    va_list argp;
    va_start(argp, fmt);

    for(i = 0; fmt[i] != '\0'; i++){
        if(fmt[i] == '%'){
            i++;
            switch(fmt[i]) {
                case '\0':
                    sentinel("Invalid format, you ended with %%.");
                    break;
                case 'd':
                    out_int = va_arg(argp, int*);
                    rc = read_int(out_int);
                    check(rc == 0, "Failed to read int.");
                    break;
                case 'c':
                    out_char = va_arg(argp, char*);
                    *out_char = fgetc(stdin);
                    break;
                case 's':
                    max_buffer = va_arg(argp, int);
                    out_string = va_arg(argp, char **);
                    rc = read_string(out_string, max_buffer);
                    check(rc == 0, "failed to read string.");
                    break;
                default:
                    sentinel("invalid format.");
            }
        } else {
            fgetc(stdin); 
        } 
        check(!feof(stdin) && !ferror(stdin), "Input error.");
    }
    va_end(argp);
    return 0;
error:
    va_end(argp);
    return -1;
}

int main(int argc, char* argv[])
{
    char *first_name = NULL;
    char *last_name = NULL;
    char initial = ' ';
    int age = 0;
    
    printf("What's your first name? ");
    int rc = read_scan("%s", MAX_DATA, &first_name);
    check(rc == 0, "Failed first name.");

    printf("What's your initial? ");
    rc = read_scan("%c\n", &initial);
    check(rc == 0, "Failed initial.");

    printf("What's your last name? ");
    rc = read_scan("%s", MAX_DATA, &last_name);
    check(rc == 0, "Failed last name.");

    printf("how old are you? ");
    rc = read_scan("%d", &age);

    printf("---- RESULTS ----\n");
    printf("First Name: %s\n", first_name);
    printf("Initial: %c\n", initial);
    printf("Last Name: %s\n", last_name);
    printf("Age: %d\n", age);

    print_format("---- RESULTS ----\n");
    print_format("First Name: [%s]\n", MAX_DATA, first_name);
    print_format("Initial: [%c]\n", initial);
    print_format("Last Name: [%s]\n", MAX_DATA, last_name);
    print_format("Age: [%d]\n", age);
    
    free(first_name);
    free(last_name);

    if(+0 > 0){
       printf("1111111111111");  
    }
    return 0;
error:
    return -1;
}
