#include <ctype.h>
#include <stdio.h>
#include <string.h>

inline int can_print_it(char);
void print_letters(char*, unsigned int);

void print_arguments(int argc, char* argv[]){
    int i = 0;
    for(i = 0; i < argc; i++){
        print_letters(argv[i], strlen(argv[i])); 
    }
}

void print_letters(char arg[], unsigned int max_len){
    int i = 0;
    for(i = 0; i < max_len; i++){
        char ch = arg[i];
        if(isalpha(ch) || isblank(ch)){
        /*if(can_print_it(ch)){*/
            printf("'%c' == %d ", ch, ch);
        }
    }
    printf("\n");
}

int can_print_it(char ch){
    return isalpha(ch) || isblank(ch);
}

int main(int argc, char* argv[])
{
    print_arguments(argc, argv);
    return 0;
}