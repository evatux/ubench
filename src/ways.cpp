#include <stdlib.h>
#include <stdio.h>

#include "utils/timer.hpp"
#include "utils/utils.hpp"

#include "jit/jit_generator.hpp"

namespace ways {

using namespace Xbyak;
using namespace jit;

struct reader_code_t: jit_generator_t {
    const char *name() const override { return "reader_code_t"; }

    reader_code_t(int size, int step, int niter): size_(size), niter_(niter), step_(step) {
        assert(size_ % (64 * step_) == 0);
        generate();
    }

    void iter() {
        int unroll = MAX2(1, MIN2(size_ / step_ / 64, 64));

        xor_(reg_offt, reg_offt);
        mov(reg_i, size_ / 64 / unroll / step_);

        Label l_step;
        L(l_step);

        auto acc = [](int id) { return Vmm(8 + (id % 8)); };

        for (int ur = 0; ur < unroll; ++ur) {
            for (int j = 0; j < 2; ++j) {
                auto addr = ptr[reg_ptr + reg_offt + ur * step_ * 64 + j * 32];
                auto vmm = Vmm((ur + j) % 8);
                if (1) {
                    vmovups(vmm, addr);
                } else {
                    vmovups(addr, vmm);
                }
                vpaddd(acc(ur), acc(ur), vmm);
            }
        }

        add(reg_offt, step_ * unroll * 64);
        dec(reg_i);
        jnz(l_step);
    }

    void generate() {
        xor_(reg_offt, reg_offt);
        Label l_iter;

        mov(reg_iter, niter_);
        L(l_iter);

        iter();

        dec(reg_iter);
        jnz(l_iter);

        ret();
    }

    int size_, niter_, step_;
    Reg64 reg_ptr = abi_param1;
    Reg64 reg_offt = r8;
    Reg64 reg_iter = r9;
    Reg64 reg_i = r10;

    using Vmm = Ymm;
};

void fill_buffer_with_offsets(char *buffer, int size) {
    int NB = size / 64;

    static int gen = -1;
    if (gen == -1) {
        const char *env = getenv("GEN");
        gen = env ? atoi(env) : 1;
    }
    int cur = 0;
    for (int nb = 0; nb < NB; ++nb) {
        int next = (cur + gen) % NB;
        // int next = (cur + 1) % NB;
        *(size_t *)(buffer + cur * 64) = next * 64;
        cur = next;
    }
}

void cache_props(int size) {
    const char *step_env = getenv("STEP");
    int step = step_env ? atoi(step_env) : 1;

    int64_t big_size = (int64_t)size * step;
    if (big_size >= (((int64_t)1)<<31)) { printf("too big\n"); return; }

    size *= step;

    const int N = size / (int)sizeof(size_t);
    const int ntimes = MAX2(10000 / MAX2((size / 8196), 1), 1);

    reader_code_t reader_code(size, step, ntimes);
    auto reader = (void (*)(void*))reader_code.getCode();

    char *array = (char *)zmalloc(size, PAGE_4K);
    fill_buffer_with_offsets(array, size);

    ztimer_t t;
    t.start();
    reader(array);
    t.stop(ntimes);

    size /= step;

    double ns = t.sec() * 1e9;
    double ts = (double)t.ticks();
    printf("size:%5s bytes\tstep:%4d\ttime:%10.2g ns\tticks:%10.2g\t", print_int(size), step, ns, ts);
    printf("time(per_cl):%10.2g ns\tticks(per_cl):%10.2g\n", ns * 64 / size, ts * 64 / size);

    zfree(array);
}

void doit(int argc, char **argv) {
    for (int a = 0; a < argc; ++a) {
        int size = parse_int(argv[a]);
        cache_props(size);
    }
}

}
