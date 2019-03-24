#include <stdio.h>

int main(int argc, char* argv[])
{
    int bugs = 100;
    double bug_rate = 1.2;
    
    printf("You have %d bugs at the imaginary rate of %f.\n", bugs, bug_rate);
    
    unsigned long u_universe_of_defects = (1 << 32) - 1;
    printf("The entire universe has %u bugs.\n", u_universe_of_defects);
    printf("The entire universe has %lu bugs.\n", u_universe_of_defects);

    long universe_of_defects = (1 << 31) - 1;
    printf("The entire universe has %ld bugs\n", universe_of_defects);
    
    double expected_bugs = bugs * bug_rate;
    printf("You are expected to have %f bugs.\n", expected_bugs);

    double part_of_universe = expected_bugs / universe_of_defects;
    printf("That is only a %e portion of the universe.\n", part_of_universe);

    char nul_byte = '\0';
    int care_percentage = bugs * nul_byte;
    printf("Which means you should care %d%%.\n", care_percentage);

    /*printf("%c\n", nul_byte);*/
    /*printf("%s\n", nul_byte);*/

    /*int zero = 0;*/
    /*printf("%c\n", zero);*/
    /*printf("%s\n", zero);       // 以为会segment fault，但是貌似gcc printf里面会检查空指针,*/
    
    return 0;
}
