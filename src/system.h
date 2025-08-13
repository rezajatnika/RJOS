#ifndef RJOS_SYSTEM_H
#define RJOS_SYSTEM_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes the system state by capturing the current monotonically increasing time.
 * This function initializes the start time in nanoseconds from a monotonic clock.
 * If the clock retrieval fails, the start time is set to 0.
 */
void system_init(void);

/**
 * @brief Retrieves the system uptime in microseconds since the system initialization.
 * This function calculates the elapsed time in microseconds based on the
 * monotonic clock. If the system was not initialized through `system_init`,
 * it performs a lazy initialization.
 *
 * @return The elapsed system uptime in microseconds as a 64-bit unsigned integer.
 */
uint64_t micros64(void);

/**
 * @brief Retrieves the elapsed time in microseconds since the system start.
 * This function returns the lower 32 bits of the microsecond-resolution timestamp
 * computed from the monotonic clock. If the monotonic clock retrieval fails,
 * the result is undefined.
 *
 * @return Elapsed time in microseconds as a 32-bit unsigned integer.
 */
uint32_t micros(void);

/**
 * @brief Retrieves the elapsed time since system initialization in milliseconds.
 * This function returns the time in milliseconds by dividing the result of
 * `micros64()` by 1000. If the system was not initialized, the function
 * attempts to initialize lazily.
 *
 * @return The elapsed time in milliseconds as a 64-bit unsigned integer
 * since the system's start time.
 */
uint64_t millis64(void);

/**
 * @brief Retrieves the elapsed time since system initialization in milliseconds.
 * This function returns the lower 32 bits of the millisecond-resolution timestamp
 * computed from the monotonic clock. If the system was not initialized, the function
 * attempts to initialize lazily.
 *
 * @return The elapsed time in milliseconds as a 32-bit unsigned integer
 * since the system's start time.
 */
uint32_t millis(void);

#ifdef __cplusplus
}
#endif

#endif
