#ifndef __LCTHW_LIST_ARGOS_H__
#define __LCTHW_LIST_ARGOS_H__

#include <lcthw/list.h>

typedef int (*List_compare)(const void*, const void*);

List *List_merge_sort(const List * const word, List_compare cmp);

int List_bubble_sort(List *word, List_compare cmp);

#endif
