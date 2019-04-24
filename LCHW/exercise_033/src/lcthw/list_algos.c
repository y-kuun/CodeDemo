#include <lcthw/list_algos.h>
#include <lcthw/dbg.h>

void ListNode_swap(ListNode *lhs, ListNode *rhs){
    if(lhs && rhs)
    {
        lhs->prev->next = rhs;
        rhs->prev = lhs->prev;
        
        rhs->next->prev = lhs;
        lhs->next = rhs->next;

        rhs->next = lhs;
        lhs->prev = rhs;
    }
}

List *List_bubble_sort(List *words, List_compare cmp)
{
    int num = words->count;
    while(num--){
        int swap_flag = 0;
        ListNode *cur = words->first;
        while(cur->next)
        {
            if(cmp(cur->value, cur->next->value) > 0)
            {
                // 为了保障节点的完整性需要特殊处理收尾交换
                if(cur->prev == NULL)
                {
                    words->first = cur->next;
                }
                if(cur->next->next == NULL)
                {
                    words->last = cur;
                }
                ListNode_swap(cur, cur->next);
                swap_flag = 1;
            }
        }
        // 判断没有交换立刻退出
        if(swap_flag) break;
    }
    return words;
}

List *List_merge_sort(List *words, List_compare cmp)
{
    if(words->count < 2) return words;
    if(words->count == 2){
        if(cmp(words->first, words->last) > 0)
        {
            ListNode *tmp = words->last;
            words->last = words->first;
            words->first = tmp;
        }
        return words;
    }
    
    ListNode *mid = NULL;
    int i = 0;
    for(i = 0, mid=words->first; i < words->count; i=i+2)
    {
        mid = mid->next;
    }
    List left = {.first = words->first, .last = mid, .count = i / 2};
    List right = {.first = mid->next,
                  .last = words->last,
                  .count = words->count - i / 2};
    List_merge_sort(&left, cmp);
    List_merge_sort(&right, cmp);
    memset(words, '\0', sizeof(List));

    ListNode *left_cur = left.first;
    ListNode *right_cur = right.first;
    ListNode *cur = NULL;
    while(left_cur && right_cur){
        if(cmp(left_cur, right_cur) > 0)
        {
            cur = left_cur;
            left_cur = left_cur->next;
        }
        else
        {
            cur = right_cur;
            right_cur = right_cur->next;
        }
        if(words->first == NULL)
        {
            words->first = cur;
            words->last = cur;
        }
        words->last->next = cur;
        cur->prev = words->last;
        words->last = cur;
    }
    cur = left_cur =! NULL ? left_cur : right_cur;
    while(cur)
    {
        words->last->next = cur;
        cur->prev = words->last;
        words->last = cur;
        cur = cur->next;
    }
    if(words->last) words->last->next = NULL;
    return words;
}
