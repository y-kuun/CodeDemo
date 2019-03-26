#include <stdio.h>

char lower(char);

int main(int argc, char* argv[])
{
    if(argc != 2){
        printf("ERROR: You need one argument.\n");
        return 1;
    }
    int i = 0;
    char letter;
    /*for(i = 0; argv[1][i] != "\0"; i++){*/
    for(i = 0, letter='a'; argv[1][i] != '\0'; i++){
        /*char letter = argv[1][i]; */
        letter = lower(argv[1][i]); 
        switch(letter){
            case 'a': //fallthrough 
            /*case 'A': */
                printf("%d: 'A'\n", i);
                break;
            case 'e': //fallthrough 
            /*case 'E': */
                printf("%d: 'E'\n", i);
                break;
            case 'i': //fallthrough 
            /*case 'I': */
                printf("%d: 'I'\n", i);
                break;
            case 'o': //fallthrough 
            /*case 'O': */
                printf("%d: 'O'\n", i);
                break;
            case 'u': //fallthrough 
            /*case 'U': */
                printf("%d: 'U'\n", i);
                break;
            case 'y': //fallthrough 
            /*case 'Y': */
                if(i > 2){
                    printf("%d: 'Y'\n", i);
                }
                break;
            default:
                printf("%d: %c is not a vowel\n", i, letter);
        }
    }
    return 0;
}

char lower(char ch){
    return 'a' + ch - 'A';
}
