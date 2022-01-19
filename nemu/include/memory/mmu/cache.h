#ifndef __CACHE_H__
#define __CACHE_H__

#include "nemu.h"
#include <stdlib.h>

#ifdef CACHE_ENABLED

typedef struct
{
    bool validBit;
    uint32_t tag;
    uint8_t data[64];
} CacheLine;

#define NUM_OF_LINE 1024
#define LINE_PER_SET 8
#define BYTE_PER_LINE 64

// init the cache
void init_cache();

// write data to cache
void cache_write(paddr_t paddr, size_t len, uint32_t data);

// read data from cache
uint32_t cache_read(paddr_t paddr, size_t len);

#endif

#endif
