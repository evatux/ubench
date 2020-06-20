#ifndef UTILS_UTILS_HPP
#define UTILS_UTILS_HPP

#include <stdlib.h>
#include <stddef.h>

#define MIN2(a,b) (((a)<(b))?(a):(b))
#define MAX2(a,b) (((a)>(b))?(a):(b))

#define PAGE_4K 4096
#define PAGE_2M 2097152

void *zmalloc(size_t size, int align);
void zfree(void *ptr);

#endif
