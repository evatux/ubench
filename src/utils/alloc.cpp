#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>

#include "utils.hpp"

void *zmalloc(size_t size, int align) {
#if 1
    void *ptr;
    int rc = posix_memalign(&ptr, align, size);
    return rc == 0 ? ptr : nullptr;
#else
    size = rnd_up(size, PAGE_2M) + PAGE_2M;
    void *addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
//    printf("%p\n", addr);
    *(size_t *)addr = size;
//    printf("%p\n", addr);
    return (void*)((char *)addr + PAGE_2M);
#endif
}

void zfree(void *ptr) {
#if 1
    free(ptr);
#else
    size_t *addr = (size_t*)((char *)ptr - PAGE_2M);
    munmap(addr, *addr);
#endif
}
