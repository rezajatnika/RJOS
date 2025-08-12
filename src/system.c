#include "system.h"
#include <time.h>

static struct {
    uint64_t start_time_ns;
} state;

static uint64_t ts_to_ns(struct timespec *ts) {
    return ts->tv_sec * 1000000000UL + ts->tv_nsec;
}

void system_init(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    state.start_time_ns = ts_to_ns(&ts);
}

uint64_t micros64(void) {
    static uint64_t start_us = 0;
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    uint64_t now_us = ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
    if (start_us == 0) {
        start_us = now_us;
    }
    return now_us - start_us;
}

uint64_t millis64(void) {
    return micros64() / 1000;
}

uint32_t micros(void) {
    return micros64() & 0xFFFFFFFF;
}

uint32_t millis(void) {
    return millis64() & 0xFFFFFFFF;
}
