#include <stdio.h>
#include <ctype.h>
#include "dbg.h"

int print_a_message(const char *msg){
    printf("A STRING: %s\n", msg);
    return 0;
}

int print_a_message_safe(const char *msg, const int n){
    printf("A STRING: %s %d\n", msg, n);
    return 0;
}

int uppercase(const char *msg){
    int i = 0;

    for(i = 0; msg[i] != '\0'; i++){
	printf("%c", toupper(msg[i]));
    }
    printf("\n");

    return 0;
}

int uppercase_safe(const char *msg, const int n){
    int i = 0;

    for(i = 0; msg[i] != '\0' && i < n; i++){
	printf("%c", toupper(msg[i]));
    }
    printf("\n");

    return 0;
}

int lowercase(const char *msg){
    int i = 0;

    for(i = 0; msg[i] != '\0'; i++){
	printf("%c", toupper(msg[i]));
    }
    printf("\n");

    return 0;
}

int lowercase_safe(const char *msg, const int n){
    int i = 0;

    for(i = 0; msg[i] != '\0' && i < n; i++){
	printf("%c", toupper(msg[i]));
    }
    printf("\n");

    return 0;
}

int fail_on_purpose(const char *msg){
    return 1;
}

int fail_on_purpose_safe(const char *msg, const int n){
    return 1;
}

