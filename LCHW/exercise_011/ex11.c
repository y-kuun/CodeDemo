#include <stdio.h>

int main(int argc, char* argv[])
{
    int i = 0;
    /*int num_states = 40;*/
    int num_states = 4;
    char *states[] = {
        "California", "Oregon",
        "Washington", "Texas",
    };

    i = argc;
    while(i > 0){
        i--;
        printf("arg %d: %s\n", i, argv[i]);
    }
    
    i = 0;
    // only exchanged the pointer
    while(i < num_states && i < argc){
        printf("state prev %d: %s\n", i, states[i]);
        states[i] = argv[i];
        printf("state after %d: %s\n", i, states[i]);
        i++;
    }
    return 0;
}
