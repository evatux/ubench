#include <stdlib.h>

#include "utils.hpp"

void *zmalloc(size_t size, int align) {
    void *ptr;
    int rc = posix_memalign(&ptr, align, size);
    return rc == 0 ? ptr : nullptr;
}

void zfree(void *ptr) {
    free(ptr);
}
