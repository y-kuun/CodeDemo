#include <stdio.h>

int main(int argc, char* argv[])
{
    int distance = 100;
    float power = 2.345f;
    double super_power = 56789.4532;
    char initial = 'A';
    char first_name[] = "Yukun";
    char last_name[] = "Du";

    /*printf("empty string [%s]", distance);*/
    /*printf("empty string [%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s]");*/
    printf("You are %d miles away.\n", distance); 
    printf("You have %f levels of power.\n", power);
    printf("You have %f awesome super powers.\n", super_power);
    printf("I have an initial %c.\n", initial);
    printf("I have a first name %s.\n", first_name);
    printf("I have a last name %s.\n", last_name);
    printf("My whole anme is %s %c %s.\n", first_name, initial, last_name);
    printf("double vs float: %lf vs %f\n", 56789.4532, 56789.4532);
    printf("double vs float: %lf vs %f\n", 56789.4532f, 56789.4532f);
    printf("e: [% e] [% e]\n", 56789.4532, -56789.4532);
    printf("e: [%+e] [%+e]\n", 56789.4532, -56789.4532);
    printf("empty string [%s]\n", "");
    printf("dec: %5d\noct: %5o\t%#5o\nhex: %5x\n", distance, distance, distance);
    return 0;
}
