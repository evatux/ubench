#ifndef UTILS_UTILS_HPP
#define UTILS_UTILS_HPP

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <math.h>

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

inline int parse_int(const char *str) {
    char mod = ' ';
    double size = 1;
    int count = sscanf(str, "%lf%c", &size, &mod);
    if (mod == 'K' || mod == 'k') size *= 1024;
    if (mod == 'M' || mod == 'm') size *= 1024 * 1024;
    if (mod == 'G' || mod == 'g') size *= 1024 * 1024;
    return (int)size;
}

inline const char *print_int(int size) {
    char *buffer = (char *)malloc(32);
    double sz = size;
    char mod = ' ';
    if (sz >= 1024) { sz /= 1024.; mod = 'k'; }
    if (sz >= 1024) { sz /= 1024.; mod = 'm'; }
    if (sz >= 1024) { sz /= 1024.; mod = 'g'; }
    if (mod == ' ') sprintf(buffer, "%d", size);
    else if (sz - floor(sz) < 1e-7) sprintf(buffer, "%d%c", (int)sz, mod);
    else sprintf(buffer, "%.2f%c", sz, mod);
    return buffer;
}

#endif
