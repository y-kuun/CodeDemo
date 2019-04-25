#include <lcthw/list_algos.h>
#include <lcthw/dbg.h>

void ListNode_swap(ListNode *lhs, ListNode *rhs)
{
    if(lhs && rhs)
    {
        if(lhs->prev) lhs->prev->next = rhs;
        rhs->prev = lhs->prev;
        
        if(rhs->next) rhs->next->prev = lhs;
        lhs->next = rhs->next;
        
        rhs->next = lhs;
        lhs->prev = rhs;
    }
}

int List_bubble_sort(List *words, List_compare cmp)
{
    int num = words->count;
    ListNode *cur = words->first;
    while(num-- && cur != NULL){
        int swap_flag = 0;
        while(cur->next)
        {
            if(cmp(cur->value, cur->next->value) > 0)
            {
                // debug("exchanged for [%s] [%s]", cur->value, cur->next->value);
                if(cur->prev == NULL)
                {
                    // first node exchanged
                    words->first = cur->next;
                }
                if(cur->next->next == NULL)
                {
                    // last node exchanged
                    words->last = cur;
                }
                ListNode_swap(cur, cur->next);
                swap_flag = 1;
                // since already swapped compare cur with next's next in next round
            }
            else
            {
                // debug("ignore for [%s] [%s]", cur->value, cur->next->value);                
                // compare next with next's next in next round
                cur = cur->next;
            }
        }
        cur = words->first;
        // debug("=====================");
        // 判断没有交换立刻退出
        if(swap_flag == 0) break;
    }
    return 0;
}

List *List_merge_sort(const List * const words, List_compare cmp)
{
    // debug("processing List with [%d]", words->count);
    List *res = NULL;
    if(words->count < 2)
    {
        res = List_create();
        if (words->first) List_push(res, words->first->value);
        return res;
    }

    int i = 0;
    ListNode *mid = NULL;
    for(i = 0, mid=words->first; i < words->count; i=i+2)
    {
        mid = mid->next;
    }
    List left = {.first = words->first, .last = mid->prev, .count = i / 2};
    List right = {.first = mid,
                  .last = words->last,
                  .count = words->count - i / 2};
    // sh
    // left.last->next = right.first->prev = NULL;
    
    List *lhs = List_merge_sort(&left, cmp);
    List *rhs = List_merge_sort(&right, cmp);
    // debug("Finish  List with [%d]", words->count);    
    res = List_create();
    
    ListNode *left_cur = lhs->first;
    ListNode *right_cur = rhs->first;
    ListNode *cur = NULL;
    
    while(left_cur && right_cur){
        if(cmp(left_cur->value, right_cur->value) < 0)
        {
            cur = left_cur;
            left_cur = left_cur->next;
        }
        else
        {
            cur = right_cur;
            right_cur = right_cur->next;
        }
        // debug("appending [%s]", cur->value);        
        List_push(res, cur->value);
    }
    cur = left_cur =! NULL ? left_cur : right_cur;
    while(cur)
    {
        // debug("appending [%s]", cur->value);
        List_push(res, cur->value);
        cur = cur->next;
    }
    // debug("======================== lhs");
    List_destroy(lhs);
    // debug("======================== lhs");    
    List_destroy(rhs);
    return res;;
}
