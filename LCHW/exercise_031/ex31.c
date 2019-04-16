#include <unistd.h>

int crash(int i){
    if(i < 0) crash(0);
    if(i > 0) crash(i-1);
    free((void*)i);
    return 1 / i;
}

int main(int argc, char* argv[])
{
    /* int i = 0; */
    /* while(i < 100){ */
    /*     usleep(3000); */
    /* } */
    crash(100);
    return 0;
}

