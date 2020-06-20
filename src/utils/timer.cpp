#include "timer.hpp"

#include "utils.hpp"

#include <chrono>

static inline double ms_now() {
    auto time = std::chrono::high_resolution_clock::now().time_since_epoch();
    return std::chrono::duration<double, std::milli>(time).count();
}

#if !defined(USE_RDPMC) || defined(_WIN32)
unsigned long long ticks_now() {
    return (unsigned long long)0;
}
#else
unsigned long long ticks_now() {
    unsigned eax, edx, ecx;

    ecx = (1 << 30) + 1;
    __asm__ volatile("rdpmc" : "=a"(eax), "=d"(edx) : "c"(ecx));

    return (unsigned long long)eax | (unsigned long long)edx << 32;
}
#endif

void ztimer_t::reset() {
    times_ = 0;
    for (int i = 0; i < n_modes; ++i) ticks_[i] = 0;
    ticks_start_ = 0;
    for (int i = 0; i < n_modes; ++i) ms_[i] = 0;
    ms_start_ = 0;

    start();
}

void ztimer_t::start() {
    ticks_start_ = ticks_now();
    ms_start_ = ms_now();
}

void ztimer_t::stop(int add_times) {
    if (add_times == 0) return;

    long long d_ticks = ticks_now() - ticks_start_; /* FIXME: overflow? */
    double d_ms = ms_now() - ms_start_;

    ticks_start_ += d_ticks;
    ms_start_ += d_ms;

    ms_[ztimer_t::avg] += d_ms;
    ticks_[ztimer_t::avg] += d_ticks;

    d_ticks /= add_times;
    d_ms /= add_times;

    ms_[ztimer_t::min] = times_ ? MIN2(ms_[ztimer_t::min], d_ms) : d_ms;
    ms_[ztimer_t::max] = times_ ? MAX2(ms_[ztimer_t::max], d_ms) : d_ms;

    ticks_[ztimer_t::min] = times_ ? MIN2(ticks_[ztimer_t::min], d_ticks) : d_ticks;
    ticks_[ztimer_t::max] = times_ ? MAX2(ticks_[ztimer_t::max], d_ticks) : d_ticks;

    times_ += add_times;
}

ztimer_t &ztimer_t::operator=(const ztimer_t &rhs) {
    if (this == &rhs) return *this;
    times_ = rhs.times_;
    for (int i = 0; i < n_modes; ++i)
        ticks_[i] = rhs.ticks_[i];
    ticks_start_ = rhs.ticks_start_;
    for (int i = 0; i < n_modes; ++i)
        ms_[i] = rhs.ms_[i];
    ms_start_ = rhs.ms_start_;
    return *this;
}
