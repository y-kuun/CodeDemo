#include <stdlib.h>
#include "dbg.h"

#define MAX_DATA 100

typedef enum EyeColor {
    BLUE_EYES, GREEN_EYES, BROWN_EYES, BLACK_EYES, OTHER_EYES
} EyeColor;

const char *EYE_COLOR_NAMES[] = {
    "Blue", "Green", "Brown", "Black", "Other"
};

typedef struct Person {
    int age;
    char first_name[MAX_DATA];
    char last_name[MAX_DATA];
    EyeColor eyes;
    float income;
} Person;

int main(int argc, char* argv[])
{
    Person you = {.age = 0};
    char input_string[MAX_DATA];
    int i = 0;
    char *in = NULL;
    char **invalid = malloc(sizeof(char*));
    
    printf("What's your First Name? ");
    in = fgets(you.first_name, MAX_DATA - 1, stdin);
    check(in != NULL, "Failed to read first name.");

    printf("What's your Last Name? ");
    in = fgets(you.last_name, MAX_DATA - 1, stdin);
    check(in != NULL, "Failed to read last name.");

    int rc;
    printf("How old are you? ");
    in = fgets(input_string, MAX_DATA - 1, stdin);
    you.age = strtol(input_string, invalid, 10);
    /*you.age = atoi(input_string);*/
    /*int rc = fscanf(stdin, "%d", &you.age);*/
    check(**invalid == '\0', "Failed to read age. You have to enter a number.");
    clean_errno();

    printf("What color are your eyes:\n");
    for(i = 0; i < OTHER_EYES; i++){
        printf("%d) %s\n", i + 1, EYE_COLOR_NAMES[i]);
    }
    printf(">" );

    int eyes = -1;
    rc = fscanf(stdin, "%d", &eyes);
    check(rc > 0, "Failed to eye color. You have to enter a number.");
    
    you.eyes = eyes - 1;
    check(you.eyes <= OTHER_EYES && you.eyes >= 0, "Do it right, that's not an option.");
    
    printf("How much do you make an hour? ");
    rc = fscanf(stdin, "%f", &you.income);
    check(rc > 0, "Enter a floating point number.");
    
    printf("------ RESULTS ------\n");
    
    printf("First Name: %s", you.first_name);
    printf("Last Name: %s", you.last_name);
    printf("Age: %d\n", you.age);
    printf("Eyes: %s\n", EYE_COLOR_NAMES[you.eyes]);
    printf("Income: %f\n", you.income);
    
    if(invalid) free(invalid);
    return 0;
error:
    // free(*invalid) double free
    if(invalid) free(invalid);
    return -1;
}
