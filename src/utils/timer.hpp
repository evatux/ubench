#ifndef UTILS_TIMER_HPP
#define UTILS_TIMER_HPP

struct ztimer_t {
    enum mode_t { min = 0, avg = 1, max = 2, n_modes };

    ztimer_t() { reset(); }

    void reset(); /** fully reset the measurements */

    void start(); /** restart timer */
    void stop(int add_times = 1); /** stop timer & update statistics */

    void stamp(int add_times = 1) { stop(add_times); }

    int times() const { return times_; }

    double total_ms() const { return ms_[avg]; }

    double ms(mode_t mode = min) const {
        if (!times()) return 0; // nothing to report
        return ms_[mode] / (mode == avg ? times() : 1);
    }

    double sec(mode_t mode = min) const { return ms(mode) / 1e3; }

    long long ticks(mode_t mode = min) const {
        if (!times()) return 0; // nothing to report
        return ticks_[mode] / (mode == avg ? times() : 1);
    }

    ztimer_t &operator=(const ztimer_t &rhs);

    int times_;
    long long ticks_[n_modes], ticks_start_;
    double ms_[n_modes], ms_start_;
};

#endif
