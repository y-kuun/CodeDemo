#ifndef hashmap_algos_h
#define hashmap_algos_h

#include <stdint.h>

// named after Glenn Fowler, Phong Vo and Landon Curt Noll, fast and good hash
uint32_t Hashmap_fnv1a_hash(void *data);

// bad hash but good for learning
uint32_t Hashmap_adler32_hash(void *data);

// very fast but not good hash value
uint32_t Hashmap_djb_hash(void *data);

// default hash
uint32_t default_hash(void *data);

#endif
