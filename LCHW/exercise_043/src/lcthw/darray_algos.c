#include <string.h>
#include <stdlib.h>
#include <lcthw/darray_algos.h>

// #define USE_STANDARD_LIB

static void exchange(void **content, int dst, int src)
{
    void *tmp = content[dst];
    content[dst] = content[src];
    content[src] = tmp;
}

#ifndef USE_STANDARD_LIB
static int qsort_helper(void **content, int lhs, int rhs, DArray_compare cmp)
{
    int l, r, rs;
    rs = 0;
    check(content != NULL, "valid pointer needed!");
    if(lhs >= rhs) return 0;
    void *pivot = content[lhs];
    l = lhs;
    r = rhs;
    while(l < r)
    {
        // debug("current l %d r %d", l, r);
        while(l < r && cmp(&pivot, &content[r]) <= 0) r--;
        content[l] = content[r];
        while(l < r && cmp(&pivot, &content[l]) > 0) l++;
        content[r] = content[l];
    }
    // debug("current l %d r %d", l, r);
    content[l] = pivot;
    rs = qsort_helper(content, lhs, l - 1, cmp);
    check(rs == 0, "qsort failed for left part");
    rs = qsort_helper(content, l + 1, rhs, cmp);
    check(rs == 0, "qsort failed for right part");
    return rs;
error:
    return rs;
}

static int heapsort_helper(void **content, int lhs, int rhs, DArray_compare cmp)
{
    int cur_len, max;
    void **heap_content;
    cur_len = max = rhs - lhs + 1;
    heap_content = calloc(1, max * sizeof(void*));
    // build a small heap
    check_mem(heap_content);
    memcpy(heap_content, content, max * sizeof(void*));
    while(cur_len > 0)
    {
        int mid = 0;
        int lidx, ridx, sidx;
        void *lchild, *rchild, *root;
        // print_array(heap_content, max - 1);
        for(mid = cur_len / 2 - 1; mid >= 0; mid--)
        {

            lidx = mid * 2 + 1;
            ridx = mid * 2 + 2 < cur_len ? mid * 2 + 2 : lidx;
            // debug("lidx %d mid %d rid %d cur len %d", lidx, mid, ridx, cur_len);
            root = heap_content[mid];
            lchild = heap_content[lidx];
            rchild = heap_content[ridx];
            sidx = cmp(&lchild, &rchild) > 0 ? ridx : lidx;
            if(cmp(&root, &heap_content[sidx]) > 0){
                exchange(heap_content, mid, sidx);
            }
        }
        // debug("heap_content[0] [%s] wins this around", (char*)heap_content[0]);
        // small heap get the smallest one
        content[max - cur_len] = heap_content[0];
        // the elements who wins should be poped and pushed back
        exchange(heap_content, 0, cur_len - 1);
        cur_len -= 1;
    }
    if(heap_content) free(heap_content);
    return 0;
error:
    if(heap_content) free(heap_content);
    return -1;
}

static void **content_copy(void **content, int start, int n)
{
    void **dummy_content = calloc(1, n * sizeof(void*));
    // build a small heap
    check_mem(dummy_content);
    memcpy(dummy_content, content + start, n * sizeof(void*));
    return dummy_content;
error:
    return NULL;
}

static int mergesort_helper(void **content, int lhs, int rhs, DArray_compare cmp)
{
    if(lhs >= rhs) return 0;
    if(lhs + 1 == rhs)
    {
        if(cmp(&content[lhs], &content[rhs]) > 0)
        {
            exchange(content, lhs, rhs);
        }
        return 0;
    }
    int mid = (lhs + rhs) / 2;
    mergesort_helper(content, lhs, mid, cmp);
    mergesort_helper(content, mid + 1, rhs, cmp);
    int llen = mid - lhs + 1;
    int rlen = rhs - mid;
    void **lcontent, **rcontent;
    lcontent = rcontent = NULL;
    lcontent = content_copy(content, lhs, llen);
    check_mem(lcontent);
    rcontent = content_copy(content, lhs + llen, rlen);
    check_mem(rcontent);
    void **cp = content + lhs;
    void **lp = lcontent;
    void **rp = rcontent;
    // debug("merging l %d(%d) r %d(%d)", llen, lhs, rlen, lhs + llen);
    while(lp - lcontent < llen && rp - rcontent < rlen)
    {
        // debug("comparing l %s r %s", *lp, *rp);
        if(cmp(lp, rp) > 0)
        {
            // debug("cp right");
            // error: expression is not assignable
            // cp++ = rp++;
            *cp++ = *rp++;
        }
        else
        {
            // debug("cp left");            
            *cp++ = *lp++;
        }
    }
    while(lp - lcontent < llen) *cp++ = *lp++;
    while(rp - rcontent < rlen) *cp++ = *rp++;
    if(lcontent) free(lcontent);
    if(rcontent) free(rcontent);
    return 0;
error:
    if(lcontent) free(lcontent);
    if(rcontent) free(rcontent);
    return -1;
}
#endif

int DArray_qsort(DArray *array, DArray_compare cmp)
{
    debug("qsort_helper(%d) starts", DArray_count(array));
    // real idx should be passed
#ifdef USE_STANDARD_LIB
    qsort(array->contents, DArray_count(array), sizeof(void*), cmp);
    return 0;
#else
    return qsort_helper(array->contents, 0, DArray_count(array) - 1, cmp);
#endif
}

int DArray_heapsort(DArray *array, DArray_compare cmp)
{
    debug("heapsort_helper(%d) starts", DArray_count(array));
#ifdef USE_STANDARD_LIB
    // return heapsort(array->contents, DArray_count(array), sizeof(void*), cmp);        
#else
    return heapsort_helper(array->contents, 0, DArray_count(array) - 1, cmp);
#endif
}

int DArray_mergesort(DArray *array, DArray_compare cmp)
{
    debug("mergesort_helper(%d) starts", DArray_count(array));
#ifdef USE_STANDARD_LIB
    // return mergesort(array->contents, DArray_count(array), sizeof(void*), cmp);    
#else
    return  mergesort_helper(array->contents, 0, DArray_count(array) - 1, cmp);
#endif
}

void** DArray_find(DArray *array, DArray_compare cmp, void *el)
{
    void **data = array->contents;
    int lidx = 0;
    int ridx = DArray_count(array) - 1;

    // debug
    // print_array(data, ridx);

    while(lidx <= ridx){
        int mid = lidx + (ridx - lidx) / 2;
        int rc = cmp(data + mid, &el);

        char *ts0, *ts2;
        ts0 = el;
        ts2 = *(data + mid);
        // debug("mid %d %s rc %d %s cmp ", mid, ts2, rc, ts0);
        if(rc > 0)
        {
            ridx = mid - 1;
        }
        else if(rc < 0)
        {
            lidx = mid + 1;
        }
        else
        {
            return data + mid;
        }
    }
    return NULL;
}
