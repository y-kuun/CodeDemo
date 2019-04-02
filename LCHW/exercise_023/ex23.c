#include <stdio.h>
#include <string.h>
#include <time.h>
#include "dbg.h"

int normal_copy(char *from, char *to, int count){
    int i = 0;
    for(i = 0; i < count; i++){
        to[i] = from[i]; 
    }
    return i;
}


#define COPY_8(T, F) {\
    *to++ = *from++;\
    case 7: *T++ = *F++;\
    case 6: *T++ = *F++;\
    case 5: *T++ = *F++;\
    case 4: *T++ = *F++;\
    case 3: *T++ = *F++;\
    case 2: *T++ = *F++;\
    case 1: *T++ = *F++;\
}

int duffs_device(char *from, char *to, int count){
{
        int n = (count + 7) / 8; 
        switch(count % 8){
            case 0: do {
                        COPY_8(to, from)
                        /**to++ = *from++;*/
                        /*case 7: *to++ = *from++;*/
                        /*case 6: *to++ = *from++;*/
                        /*case 5: *to++ = *from++;*/
                        /*case 4: *to++ = *from++;*/
                        /*case 3: *to++ = *from++;*/
                        /*case 2: *to++ = *from++;*/
                        /*case 1: *to++ = *from++;*/
                    }  while(--n > 0);
        }
    }
    return count;
}

int zeds_device(char *from, char *to, int count){
    {
        int n = (count + 7) / 8; 
        
        switch(count % 8){
            case 0: *to++ = *from++;
again:          *to++ = *from++;
            case 7: *to++ = *from++;
            case 6: *to++ = *from++;
            case 5: *to++ = *from++;
            case 4: *to++ = *from++;
            case 3: *to++ = *from++;
            case 2: *to++ = *from++;
            case 1: *to++ = *from++;            // AddressSanitizer will have false positive here
                if(--n > 0) goto again;
        }
    }
    return count;
}

int valid_copy(char *data, int count, char expects){
    int i = 0;
    for(i = 0; i < count; i++){
        if(data[i] != expects){
            log_err("[%d] %c != %c", i, data[i], expects);
            return 0;
        }
    }
    return 1;
}


#define TIME_CONSUME ((double)(end - beg) / CLOCKS_PER_SEC)

int main(int argc, char* argv[])
{
    // 实际上把需要拷贝的长度 n， 变成 n = floor(n/8) + n % 8
    const long LEN = 1024 * 1024 * 64;
    static char from[LEN] = {'a'};
    static char to[LEN] = {'c'};
    int rc = 0;
    clock_t beg, end;
    log_info("Current LEN is %ld", LEN);
    
    beg = clock();
    memset(from, 'x', LEN);
    end = clock();
    log_info("memset for x: %lf", TIME_CONSUME);

    beg = clock();
    memset(to, 'y', LEN);
    end = clock();
    log_info("memset for y: %lf", TIME_CONSUME);

    beg = clock();
    memcpy(to, from, LEN);
    end = clock();
    log_info("memcpy : %lf", TIME_CONSUME);

    beg = clock();
    memmove(to, from, LEN);
    end = clock();
    log_info("memmove : %lf", TIME_CONSUME);

    beg = clock();
    strncpy(to, from, LEN);
    end = clock();
    log_info("strncpy : %lf", TIME_CONSUME);

    memset(to, 'y', LEN);
    beg = clock();
    check(valid_copy(to, LEN, 'y'), "Not initialized right.");
    end = clock();
    log_info("valid_copy: %lf", TIME_CONSUME);

    beg = clock();
    rc = normal_copy(from, to, LEN);
    end = clock();
    log_info("normal_copy: %lf", TIME_CONSUME);

    check(rc == LEN, "Normal copy failed: %d", rc);
    check(valid_copy(to, LEN, 'x'), "Normal copy failed");

    memset(to, 'y', LEN);
    beg = clock();
    rc = duffs_device(from, to, LEN);
    end = clock();
    log_info("duffs_device: %lf", TIME_CONSUME);

    check(rc == LEN, "Duff's device failed: %d", rc);
    check(valid_copy(to, LEN, 'x'), "Duff's device failed");

    memset(to, 'y', LEN);
    beg = clock();
    rc = zeds_device(from, to, LEN);
    end = clock();
    log_info("zeds_device: %lf", TIME_CONSUME);

    check(rc == LEN, "Zed's device failed: %d", rc);
    check(valid_copy(to, LEN, 'x'), "Zed's device failed");
    log_info("All finished!");
    return 0;
error:
    return 1;
}
