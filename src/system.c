#include "system.h"
#include <time.h>
#include <sys/types.h>

static struct {
    uint64_t start_time_ns;
} state;

static inline uint64_t ts_to_ns(const struct timespec *ts) {
    return (uint64_t)ts->tv_sec * UINT64_C(1000000000) + (uint64_t)ts->tv_nsec;
}

static inline get_monotonic_timespec(struct timespec *ts) {
#if defined(CLOCK_MONOTONIC_RAW)
    if (clock_gettime(CLOCK_MONOTONIC_RAW, ts) == 0) {
        return 0;
    }
#endif
#if defined(CLOCK_MONOTONIC)
    if (clock_gettime(CLOCK_MONOTONIC, ts) == 0) {
        return 0;
    }
#endif
#if defined(TIME_UTC)
    if (timespec_get(ts, TIME_UTC) == TIME_UTC) {
        return 0;
    }
#endif
    return -1;
}

static inline uint64_t now_ns(void) {
    struct timespec ts;
    if (get_monotonic_timespec(&ts) != 0) {
        return 0;
    }
    return ts_to_ns(&ts);
}

void system_init(void) {
    struct timespec ts;
    if (get_monotonic_timespec(&ts) == 0) {
        state.start_time_ns = ts_to_ns(&ts);
    } else {
        state.start_time_ns = 0;
    }
}

uint64_t micros64(void) {
    /* Lazy initialization if system_init() was not called. */
    if (state.start_time_ns == 0) {
        struct timespec ts;
        if (get_monotonic_timespec(&ts) == 0) {
            state.start_time_ns = ts_to_ns(&ts);
        }
    }
    uint64_t start_ns = state.start_time_ns;
    uint64_t time_ns  = now_ns();
    return (time_ns - start_ns) / UINT64_C(1000);
}

uint64_t millis64(void) {
    return micros64() / UINT64_C(1000);
}

uint32_t micros(void) {
    return (uint32_t)(micros64() & UINT64_C(0xFFFFFFFF));
}

uint32_t millis(void) {
    return (uint32_t)(millis64() & UINT64_C(0xFFFFFFFF));
}
