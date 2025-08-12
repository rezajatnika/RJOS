#include <time.h>
#include "system.h"

uint32_t millis(void) {
    static uint32_t start_ms = 0;
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    uint32_t now_ms = (uint32_t)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
    if (start_ms == 0) {
        start_ms = now_ms;
    }
    return now_ms - start_ms;
}
