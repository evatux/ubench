#include <stdlib.h>
#include <stdio.h>

#include "utils/timer.hpp"
#include "utils/utils.hpp"

#include "jit/jit_generator.hpp"

namespace cache {

using namespace Xbyak;
using namespace jit;

struct reader_code_t: jit_generator_t {
    const char *name() const override { return "reader_code_t"; }

    reader_code_t(int size, int niter): size_(size), niter_(niter) {
        assert(size_ % 64 == 0);
        generate();
    }

    void iter() {
        int unroll = MAX2(1, MIN2(size_ / 64, 64));

        xor_(reg_offt, reg_offt);
        Label l_step;

        L(l_step);
        for (int ur = 0; ur < unroll; ++ur) {
            for (int j = 0; j < 4; j += 4) {
                if (1)
                    vmovups(Zmm((ur + j) % 16), ptr[reg_ptr + reg_offt + ur * 64 + j * 16]);
                else
                    vmovups(ptr[reg_ptr + reg_offt + ur * 64 + j * 16], Zmm((ur + j) % 16));
            }
        }
        add(reg_offt, unroll * 64);
        cmp(reg_offt, size_);
        jl(l_step);
    }

    void generate() {
        Label l_iter;

        mov(reg_iter, niter_);
        L(l_iter);
        iter();

        dec(reg_iter);
        jnz(l_iter);

        ret();
    }

    int size_, niter_;
    Reg64 reg_ptr = abi_param1;
    Reg64 reg_offt = r8;
    Reg64 reg_iter = r9;
};

void cache_props(int size) {
    int ntimes = MAX2(10000, 1);
    reader_code_t reader_code(size, ntimes);
    auto reader = (void (*)(void*))reader_code.getCode();

    char *array = (char *)zmalloc(size, PAGE_4K);
    for (size_t i = 0; i < size; i += CACHE_LINE) array[i] = '1';

    ztimer_t t;
    t.start();
    reader(array);
    t.stop(ntimes);

    double ns = t.sec() * 1e9;
    double ts = (double)t.ticks();
    printf("size:%d bytes\t\ttime:%g ns\t\tticks:%g\t\t\t", size, ns, ts);
    printf("time(per_cl):%g ns\t\tticks(per_cl):%g\n", ns * 64 / size, ts * 64 / size);

    zfree(array);
}

void doit(int argc, char **argv) {
    size_t size = argc >= 1 ? atoi(argv[0]) : 64 * 1024;
    cache_props(size);
}

}
