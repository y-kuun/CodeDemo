#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void double_print(int*, char**, int, int);



int main(int argc, char* argv[])
{
    int ages[] = {23, 43, 12, 89, 2};
    char *names[] = {
        // array of poninter to char array
        "Alan", "Frank",
        "Mary", "John", "Lisa"
    };
    
    int count = sizeof(ages) / sizeof(int);
    int i = 0;

    for(i = 0; i < count; i++){
        printf("%s has %d years alive.\n", names[i], ages[i]);
    }

    printf("---\n");

    int *cur_age = ages;
    char **cur_name = names;

    for(i = 0; i < count; i++){
        printf("%s is %d years old.\n", *(cur_name + i), *(cur_age + i));
    }
   
    printf("---\n");

    for(i = 0; i < count; i++){
        printf("%s is %d years old again.\n", cur_name[i], cur_age[i]);
    }

    printf("---\n");
    printf("ages: %p names: %p\n", ages, names);
    for(cur_name=names + count - 1, cur_age=ages + count - 1; cur_age - ages >= 0; cur_name--, cur_age--){
        printf("%s lived %d years so far.\n", *cur_name, *cur_age);
    }
        
    // =======================
    int nums_len = 10;
    int *nums = malloc(nums_len * sizeof(int));
    memset(nums, 0, nums_len);
    void* vp = nums;
    *nums = 0x1002;
    printf("vp: %p %d, nums: %p %d\n", vp, *(char*)vp, nums, *nums);
    if(nums){
        free(nums); 
    }

    printf("---\n");
    double_print(ages, names, count, 0);
    printf("---\n");
    typedef void (*DP_func_p)(int*, char**, int, int);
    DP_func_p func_p = double_print;
    (*func_p)(ages, names, count, 1);

    return 0;
}

void double_print(int* narr, char* sarr[], int num, int is_reversed){
    int inc_num = 1;
    if(is_reversed){
        narr += num - 1;
        sarr += num - 1;
        inc_num = -1;
    }
    while(num > 0){
        printf("[%p %s] lived for [%p %d] year(s).\n", sarr, *sarr, narr, *narr);
        num--;
        sarr += inc_num;
        narr += inc_num;
    } 
}
