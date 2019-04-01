#include <assert.h>         // function assert included
#include <stdio.h>          // function printf included
#include <stdlib.h>         // function malloc included
#include <string.h>         // function strdup included

struct Person {
    // create struct Person has for attributes, with sizeof 24 = 8 + 4 + 4 + 4
    char *name;
    int age;
    int height;
    int weight;
};

struct Person* Person_create(char *name, int age, int height, int weight){
    // alloct heep memory of size 24, if success return the address to who
    struct Person* who = malloc(sizeof(struct Person));
    // check validity of who, if who == NULL abort this program
    assert(who != NULL);
    
    // new a heep memory to which content of name is copied to
    who->name = strdup(name);
    who->age = age;
    who->height = height;
    who->weight = weight;
    return who;
}

void Person_destroy(struct Person *who){
    // check validity of who, which is passed in as an argument, if who == NULL abort this program
    assert(who != NULL);
    // free heap memory allocated in strdup
    free(who->name);
    // free heap memory allocated in Person_create
    free(who);
}

void Person_print(const struct Person *who){
    // just pass argument by pointer, 8 bytes on the stack memory will be created
    printf("Name: %s\n", who->name);
    printf("\tAge:\t%3d\n", who->age);
    printf("\tHeight:\t%3d\n", who->height);
    printf("\tWeight:\t%3d\n", who->weight);
}

void Person_print_s(struct Person who){
    // just pass argument by value, 24 bytes on the stack memory will be created
    printf("Name: %s\n", who.name);
    printf("\tAge:\t%3d\n", who.age);
    printf("\tHeight:\t%3d\n", who.height);
    printf("\tWeight:\t%3d\n", who.weight);
}

int main(int argc, char* argv[])
{
    printf("the size of struct Person is: %lu\n", sizeof(struct Person));
    struct Person *joe = Person_create("Joe Alex", 32, 64, 140);
    struct Person *frank = Person_create("Frank Blank", 20, 72, 180);
    struct Person ykdu = {"ykdu", 28, 172, 72};
    
    printf("Joe is at memory location %p:\n", joe);
    Person_print(joe);
    printf("Joe is at memory location %p:\n", frank);
    Person_print(frank);
    printf("ykdu is at memory location %p:\n", &ykdu);
    Person_print_s(ykdu);

    // pointer access to struct attribute
    joe->age += 20;
    joe->height -= 2;
    joe->weight += 40;
    Person_print(joe);
    
    frank->age += 20;
    frank->weight += 20;
    Person_print(frank);
    /*Person_print(NULL);         // uncomment this line, then address sanitizer will warn u with a SEGV*/

    Person_destroy(joe);
    Person_destroy(frank);      // comment this line, then address sanitizer will warn u with memory leakage of 24 + 12 bytes
    /*Person_destroy(NULL);*/
    return 0;
}
