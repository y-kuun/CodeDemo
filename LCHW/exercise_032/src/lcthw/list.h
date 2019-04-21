#ifndef __LCTHW_LIST_H__
#define __LCTHW_LIST_H__

#include <stdlib.h>

typedef struct ListNode {
    struct ListNode *next;
    struct ListNode *prev;
    void *value;
} ListNode;

typedef struct List {
    ListNode *first;
    ListNode *last;
    int count;
} List;

List *List_create();
void List_destroy(List *list);
void List_clear(List *list);
void List_clear_destroy(List *list);

#define List_count(A) ((A)->count)
#define List_first(A) ((A)->first != NULL ? (A)->first->value : NULL)
#define List_last(A) ((A)->last != NULL ? (A)->last->value : NULL)

void List_push(List *list, void *value);
void *List_pop(List *list);

void List_unshift(List *list, void *value);
void *List_shift(List *list);

void *List_remove(List *list, ListNode *node);

List *List_append(List *des, List *src);
List *List_concat(List *des, List *src);
List *List_copy_shallow(List **des, List *src);
List *List_copy_deep(List **des, List *src);

#define LIST_FOREACH(L, S, M, V) ListNode *_node = NULL; \
    ListNode *V = NULL; \
    for(V = _node = L->S; _node != NULL; V = _node = _node->M)

#endif
