#ifndef UTILS_UTILS_HPP
#define UTILS_UTILS_HPP

#include <stdlib.h>
#include <stddef.h>

#define MIN2(a,b) (((a)<(b))?(a):(b))
#define MAX2(a,b) (((a)>(b))?(a):(b))

#define IMPLICATION(cause,effect) (!(cause)||(effect))

#define UNUSED(x) (void)x
#define MAYBE_UNUSED(x) UNUSED(x)

#define CACHE_LINE 64
#define PAGE_4K 4096
#define PAGE_2M 2097152

void *zmalloc(size_t size, int align);
void zfree(void *ptr);

template <typename T> T div_up(const T &x, int y) { return (x + (T)y - 1) / (T)y; }
template <typename T> T rnd_up(const T &x, int y) { return div_up(x, y) * (T)y; }
template <typename T> T rnd_dw(const T &x, int y) { return x / (T)y * (T)y; }

#endif
