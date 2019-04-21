#include <lcthw/list.h>
#include <lcthw/dbg.h>

List *List_create(){
    return calloc(1, sizeof(List));
}

void List_destroy(List *list){
    LIST_FOREACH(list, first, next, cur) {
        if(cur->prev){
            free(cur->prev);
        }
    }
    free(list->last);
    free(list);
}

void List_clear(List *list){
    LIST_FOREACH(list, first, next, cur){
        if(cur->value){
            free(cur->value);
        }
    }
}

void List_clear_destroy(List *list){
    List_clear(list);
    List_destroy(list);
}

void List_push(List *list, void *value){
    ListNode *node = calloc(1, sizeof(ListNode));
    check_mem(node);

    if(list->last == NULL){
        list->first = node;
        list->last = node;
    } else {
        list->last->next = node;
        node->prev = list->last;
        list->last = node;
    }

    list->count++;
error:
    return;
}

void *List_pop(List *list){
    ListNode *node = list->last;
    return node != NULL ? List_remove(list, node) : NULL;
}

void List_unshift(List *list, void *value){
    ListNode *node = calloc(1, sizeof(ListNode));
    check_mem(node);

    node->value = value;

    if(list->first == NULL){
        list->first = node;
        list->last = node;
    } else {
        list->first->prev = node;
        node->next = list->first;
        list->first = node;
    }

    list->count++;
error:
    return;
}

void *List_shift(List *list){
    ListNode *node = list->first;
    return node != NULL ? List_remove(list, node) : NULL;
}

void *List_remove(List *list, ListNode *node){
    void *result = NULL;

    check(list->first && list->last, "List is empty.");
    check(node, "node can't be NULL");

    if(node == list->first && node == list->last){
        list->first = list->last = NULL;
    } else if (node == list->first){
        list->first = list->first->next;
        check(list->first != NULL, "Invalid list, break list");
    } else if (node == list->last){
        list->last = list->last->prev;
        check(list->last != NULL, "Invalid list, break list");
    } else {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    list->count--;
    result = node->value;
    free(node);
 error:
    return result;
}

List *List_copy_shallow(List **des, List *src)
{
    *des = NULL;
    if(src)
    {
        *des = calloc(1, sizeof(List));
        check_mem(des);
        memcpy(*des, src, sizeof(List));
    }
    return *des;
 error:
    *des = NULL;
    return *des;
}

List *List_copy_deep(List **des, List *src)
{
    *des = NULL;
    if(src)
    {
        *des = calloc(1, sizeof(List));
        check_mem(des);
        // USE a cnt to check push result
        int list_node_cnt = 0;
        LIST_FOREACH(src, first, next, cur)
        {
            // TODO(ykdu) check push result
            List_push(*des, cur->value);
            list_node_cnt++;
            check(list_node_cnt == (*des)->count, "Failed to push");
        }
    }
    return *des;
 error:
    List_clear_destroy(*des);
    *des = NULL;
    return *des;
}

List *List_append(List *des, List *src)
{
    if( des && src)
    {
        // USE a cnt to check push result
        int list_node_cnt = 0;
        LIST_FOREACH(src, first, next, cur)
        {
            List_push(des, cur->value);
            list_node_cnt++;
            check(list_node_cnt == des->count, "Failed to push");                    }
    }
    return des;
 error:
    return NULL;
}


List *List_concat(List *des, List *src){
    if(!des && !src) return NULL;
    
    List *res = calloc(1, sizeof(List));
    check_mem(res);
    // USE a cnt to check push result
    if(des) List_append(res, des);
    if(src) List_append(res, src);    
    return res;
 error:
    List_clear_destroy(res);
    res = NULL;
    return res;
}
