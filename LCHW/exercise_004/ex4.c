#include <stdio.h>

int main(int argc, char* argv[])
{
    int age = 49;                // variable on stack, directly USE results to Undefined Behavior, 0 on Debian XX on win32
    int height = 4;
    
    printf("I am %d years old.\n", age);
    /*
	I am -16777128 years x (null) �� old.
    ==19630== Conditional jump or move depends on uninitialised value(s)
    ==19630==    at 0x4E807AD: vfprintf (vfprintf.c:1636)
    ==19630==    by 0x4E88228: printf (printf.c:33)
    ==19630==    by 0x1086EC: main (ex4.c:9)
    ==19630==
    ==19630== Use of uninitialised value of size 8
    ==19630==    at 0x4E7D19B: _itoa_word (_itoa.c:179)
    ==19630==    by 0x4E81899: vfprintf (vfprintf.c:1636)
    ==19630==    by 0x4E88228: printf (printf.c:33)
    ==19630==    by 0x1086EC: main (ex4.c:9)
    ==19630==
    ==19630== Conditional jump or move depends on uninitialised value(s)
    ==19630==    at 0x4E7D1A5: _itoa_word (_itoa.c:179)
    ==19630==    by 0x4E81899: vfprintf (vfprintf.c:1636)
    ==19630==    by 0x4E88228: printf (printf.c:33)
    ==19630==    by 0x1086EC: main (ex4.c:9)
    ==19630==
    ==19630== Conditional jump or move depends on uninitialised value(s)
    ==19630==    at 0x4E819A1: vfprintf (vfprintf.c:1636)
    ==19630==    by 0x4E88228: printf (printf.c:33)
    ==19630==    by 0x1086EC: main (ex4.c:9)
    ==19630==
    ==19630== Conditional jump or move depends on uninitialised value(s)
    ==19630==    at 0x4E80861: vfprintf (vfprintf.c:1636)
    ==19630==    by 0x4E88228: printf (printf.c:33)
    ==19630==    by 0x1086EC: main (ex4.c:9)
    ==19630==
    ==19630== Conditional jump or move depends on uninitialised value(s)
    ==19630==    at 0x4E808E2: vfprintf (vfprintf.c:1636)
    ==19630==    by 0x4E88228: printf (printf.c:33)
    ==19630==    by 0x1086EC: main (ex4.c:9)
    ==19630==
     */
    printf("I am %d inches tall.\n", height);

    return 0;
}
