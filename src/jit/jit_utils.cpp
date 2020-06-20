#include <stdio.h>
#include <stdlib.h>

#include "utils/utils.hpp"

namespace jit {

void dump_jit_code(const void *code, size_t code_size, const char *code_name) {
    static int do_dump = -1;
    if (do_dump == -1) {
        const char *e = getenv("JIT_DUMP");
        do_dump = e && e[0] == '1';
    }

    if (code && do_dump) {
        static int counter = 0;
#define MAX_FNAME_LEN 256
        char fname[MAX_FNAME_LEN + 1];
        // TODO (Roma): support prefix for code / linux perf dumps
        snprintf(fname, MAX_FNAME_LEN, "jit_dump_%s.%d.bin", code_name, counter);
        counter++;

        FILE *fp = fopen(fname, "w+");
        // Failure to dump code is not fatal
        if (fp) {
            size_t unused = fwrite(code, code_size, 1, fp);
            UNUSED(unused);
            fclose(fp);
        }
    }
#undef MAX_FNAME_LEN
}

}
