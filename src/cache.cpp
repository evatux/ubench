#include <stdlib.h>
#include <stdio.h>

#include "utils/timer.hpp"
#include "utils/utils.hpp"

void cache_props(size_t size) {
    char *p = (char *)zmalloc(size, PAGE_4K);
    printf("%zu %p\n", size, p);
    zfree(p);
}

void cache(int argc, char **argv) {
    size_t size = argc >= 1 ? atoi(argv[0]) : 64 * 1024;
    cache_props(size);
}
