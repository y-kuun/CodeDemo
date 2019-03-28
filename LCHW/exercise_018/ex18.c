#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void die(const char* message){
    if(errno){
        perror(message); 
    } else {
        printf("ERROR: %s\n", message);
    }
}

typedef int (*compare_cb)(int a, int b);

typedef int* (*compare_fb)(int*, int, compare_cb);

void swap(int *a, int *b){
    if(a == b) return;
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void qsort_helper(int* nums, int lhs, int rhs, compare_cb cmp){
    if(lhs >= rhs) return;
    int origin_l = lhs;
    int origin_r = rhs;
    int pval = nums[lhs];
    /*printf(" == %d %d %d\n", lhs, rhs, pval);*/
    while(lhs < rhs){
        while(cmp(pval, nums[rhs]) <= 0 && lhs < rhs){
            rhs--; 
        }
        swap(nums + lhs, nums + rhs);
        while(cmp(pval, nums[lhs]) >= 0 && lhs < rhs){
            lhs++; 
        }
        swap(nums + lhs, nums + rhs);
    }
    /*printf(" == %d %d %d\n", lhs, rhs, pval);*/
    /*int i = 0;*/
    /*for(i = origin_l; i < origin_r + 1; i++){*/
        /*printf("%d ", nums[i]);*/
    /*}*/
    /*printf("\n");*/
    /*printf("================\n");*/
    qsort_helper(nums, origin_l, lhs - 1, cmp);
    qsort_helper(nums, rhs + 1, origin_r, cmp);
}

int* my_qsort(int *numbers, int count, compare_cb cmp){
    int *nums = malloc(sizeof(int) * count);
    if(!nums) die("Memory Error!");
    memcpy(nums, numbers, sizeof(int) * count);
    qsort_helper(nums, 0, count - 1, cmp); 
    return nums;
}


int* bubble_sort(int *numbers, int count, compare_cb cmp){
    int *nums = malloc(sizeof(int) * count);
    if(!nums) die("Memory Error!");
    memcpy(nums, numbers, sizeof(int) * count);

    int lhs = 0;
    int rhs = count - 1;
    int changed = 1;
    while(lhs < rhs && changed){
        changed = 0;
        for(lhs = 0; lhs < rhs; lhs++){
            /*printf("cmp %d %d == %d\n", lhs, nums[lhs], nums[lhs + 1]);*/
            if(cmp(nums[lhs], nums[lhs + 1]) > 0){
                /*printf("changed!\n");*/
                swap(nums + lhs, nums + lhs + 1);
                changed = 1;
            }
        }
        /*printf("================\n");*/
        /*int i = 0;*/
        /*for(i = 0; i < count; i++){*/
            /*printf("%d ", nums[i]);*/
        /*}*/
        /*printf("\n");*/
        rhs--;
        lhs = 0;
    }
    return nums;
}

int sorted_order(int a, int b){
    return a - b;
}

int reverse_order(int a, int b){
    return b - a;
}

int strange_order(int a, int b){
    if(a == 0 || b == 0){
        return 0; 
    } else {
        return a % b;
    }
}

void test_sorting(int *numbers, int count, compare_cb cmp){
    int i = 0;
    int *sorted = bubble_sort(numbers, count, cmp);
    if(!sorted) die("Failed to sort as requested.");
    
    for(i = 0; i < count; i++){
        printf("%d ", sorted[i]);
    }
    printf("\n");
    free(sorted);
    
    /*unsigned char *data = (unsigned char*) cmp;*/
    /*for(i = 0; i < 25; i++){*/
        /*printf("%02x ", data[i]);*/
    /*}*/
    /*printf("\n");*/
}

void test_sorting_2(int *numbers, int count, compare_fb sort, compare_cb cmp){
    int i = 0;
    int *sorted = (*sort)(numbers, count, cmp);
    if(!sorted) die("Failed to sort as requested.");
    
    for(i = 0; i < count; i++){
        printf("%d ", sorted[i]);
    }
    printf("\n");
    free(sorted);
    
    /*unsigned char *data = (unsigned char*) cmp;*/
    /*for(i = 0; i < 25; i++){*/
        /*printf("%02x ", data[i]);*/
    /*}*/
    /*printf("\n");*/
}

int main(int argc, char* argv[])
{
    if(argc < 2) die("USAGE: ex18 4 3 1 5 6");

    int count = argc - 1;
    int i = 0;
    char **input = argv + 1;
    
    int *numbers = malloc(sizeof(int) * count);
    if(!numbers) die("Memory Errror!");

    for(i = 0; i < count; i++){
        // test_sorting(numbers, count, main); segv here
        numbers[i] = atoi(input[i]); 
    }

    /*test_sorting(numbers, count, sorted_order);*/
    test_sorting_2(numbers, count, bubble_sort, sorted_order);
    test_sorting_2(numbers, count, my_qsort, sorted_order);
    /*test_sorting(numbers, count, reverse_order);*/
    test_sorting_2(numbers, count, bubble_sort, reverse_order);
    test_sorting_2(numbers, count, my_qsort, reverse_order);
    /*test_sorting(numbers, count, strange_order);*/
    test_sorting_2(numbers, count, bubble_sort, strange_order);
    test_sorting_2(numbers, count, my_qsort, strange_order);
    /*test_sorting(numbers, count, main);*/
    // pc pointer to zero page
    /*test_sorting(numbers, count, NULL);*/
    
    free(numbers);

    return 0;
}
