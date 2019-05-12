#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <lcthw/radixmap.h>
#include <lcthw/dbg.h>

RadixMap *RadixMap_create(size_t max)
{
    RadixMap *map = calloc(1, sizeof(RadixMap));
    check_mem(map);

    map->contents = calloc(max + 1, sizeof(RMElement));
    check_mem(map->contents);

    map->temp = calloc(1 + max, sizeof(RMElement));
    check_mem(map->temp);

    map->max = max;
    map->end = 0;

    return map;
error:
    // avoid memory leakages
    if(map){
        if(map->contents) free(map->contents);
        if(map->temp) free(map->temp);
        free(map);
    }
    return NULL;
}

void RadixMap_destroy(RadixMap *map)
{
    if(map){
        if(map->contents) free(map->contents);
        if(map->temp) free(map->temp);
        free(map);
    }
}

#define ByteOf(x,y) (((uint8_t*)x)[(y)])

static inline void radix_sort(short offset, uint64_t max, uint64_t *source, uint64_t *dest)
{
    uint64_t count[256] = {0};
    uint64_t *cp, *sp, *end;
    uint64_t s, c;
    cp = sp = end = NULL;
    s = c = 0;
    // count oocurences of every byte value
    for(sp = source, end = source + max; sp < end; sp++)
    {
        count[ByteOf(sp, offset)]++;
    }

    // count idx is denoting the value of offset-th byte in the source element
    // after the fellowing loop, the count array's value will denoting the starting offset(idx) in the dest array, while corresponding idx in count array denotes the value for offset-th byte,
    for(s = 0, cp = count, end = count + 256; cp < end; cp++)
    {
        c = *cp;
        *cp = s;
        s += c;
    }

    for(sp = source, end = source + max; sp < end; sp++)
    {
        cp = count + ByteOf(sp, offset);
        dest[*cp] = *sp;
        ++(*cp);
    }
}

void RadixMap_sort(RadixMap *map)
{
    // radixmap sort algorithm require to sort from the lowest byte
    uint64_t *source = &map->contents[0].raw;
    uint64_t *temp = &map->temp[0].raw;
    // defaultly assuming this program running in a little endian architecture?
    radix_sort(0, map->end, source, temp);
    radix_sort(1, map->end, temp, source);
    radix_sort(2, map->end, source, temp);
    radix_sort(3, map->end, temp, source);    
}

RMElement *RadixMap_find(RadixMap *map, uint32_t to_find)
{
    // map must be sorted
    int low = 0;
    int high = map->end - 1;
    RMElement *data = map->contents;

    while(low <= high)
    {
        int mid = low + (high - low) / 2;
        uint32_t key = data[mid].data.key;

        if(to_find < key)
        {
            high = mid - 1;
        }
        else if(to_find > key)
        {
            low = mid + 1;
        }
        else
        {
            return &data[mid];
        }
    }
    return NULL;
}

int RadixMap_add(RadixMap *map, uint32_t key, uint32_t value)
{
    check(key < UINT32_MAX, "Key can't be equal to UINT32_MAX.");

    RMElement element = {.data = {.key = key, .value = value}};
    check(map->end + 1 < map->max, "RadixMap is full.");

    map->contents[map->end++] = element;
    RadixMap_sort(map);
    return 0;
error:
    return -1;
}

int RadixMap_delete(RadixMap *map, RMElement *el)
{
    check(map->end > 0, "There is nothing to delete.");
    check(el != NULL, "Can't delete a NULL element.");

    el->data.key = UINT32_MAX;
    if(map->end > 1)
    {
        RadixMap_sort(map);
    }

    map->end--;
    return 0;
error:
    return -1;
}
