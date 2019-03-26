#include <stdio.h>

int main(int argc, char* argv[])
{
    int i = 0;
    /*int num_states = 40;*/
    int num_states = 5;
    char *states[] = {
        "California", "Oregon",
        "Washington", "Texas",
        NULL,
    };
    argv[0] = states[num_states - 1];
    states[0] = argv[argc - 1];
    for(i = 0; i < argc; i++){
        printf("arg %d: %s\n", i, argv[i]);
    }
    
    for(i = 0; i < num_states && i < num_states - 1; i++){
        printf("state %d: %s\n", i, states[i]);
    }
    return 0;
}
