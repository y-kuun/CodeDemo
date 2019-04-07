#ifndef __UTILS__
#define __UTILS__

typedef int (*compress_func)(const void *unc_data, const int size, void **cdata);

typedef int (*uncompress_func)(const void *c_data, const int size, void **unc_data);

typedef int (*buff_process)(const void *, const int, void **);

typedef unsigned char Byte;

int base64(const void *, const int size, void**);
int unbase64(const void *, const int size, void**);

#endif
