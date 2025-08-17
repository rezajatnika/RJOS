#ifndef RJOS_LOGGER_H
#define RJOS_LOGGER_H

#include <stdio.h>
#include <pthread.h>

#define LOG_LEVEL_DEBUG 0
#define LOG_LEVEL_INFO  1
#define LOG_LEVEL_WARN  2
#define LOG_LEVEL_ERROR 3

/**
 * @struct logger
 * @brief A structure for managing logging functionality.
 *
 * The logger structure is designed to handle log file operations, manage the
 * log level, and enable or disable logging functionality. It provides
 * thread-safe operations by incorporating a mutex for synchronization.
 */
typedef struct logger {
    FILE *file;
    int log_level;
    int enabled;
    pthread_mutex_t mutex;
} logger_t;

/**
 * @brief Initializes the logger with the specified file and log level.
 *
 * This function initializes the global logger by setting the log file, log level,
 * and enabling the logging functionality. If the logger is already initialized,
 * it ensures that any previously opened file is closed before opening the new file.
 *
 * @param filename The name of the file to which logs will be written.
 * @param log_level The logging level for filtering log messages.
 * @return Returns 0 on success, or -1 if an error occurs (e.g., when the file fails to open).
 */
int logger_init(char *filename, int log_level);

/**
 * @brief Sets the logging level for the logger.
 *
 * This function allows the user to update the log level of the global logger.
 * The log level controls the verbosity of the logs being generated.
 *
 * @param log_level The new log level to be set for the logger. It should be
 *        an integer value corresponding to the desired verbosity.
 */
void logger_set_log_level(int log_level);

/**
 * @brief Enables or disables logging functionality.
 *
 * This function modifies the logging state of the logger by enabling or
 * disabling it based on the value provided. The operation is thread-safe and
 * ensures consistent modification of the logger's state.
 *
 * @param enabled An integer value where non-zero enables logging, and zero disables logging.
 */
void logger_enable(int enabled);

/**
 * @brief Logs a message with a specified log level.
 *
 * This function logs a formatted message to the log file associated with
 * the global logger instance. It includes a timestamp, log level, and
 * user-provided message. If logging is disabled or the log level is below
 * the configured threshold, the message is ignored.
 *
 * @param level The severity level of the log message (e.g., LOG_LEVEL_DEBUG, LOG_LEVEL_INFO).
 * @param format The format string for the log message, similar to printf.
 * @param ... Additional arguments corresponding to the format string.
 */
void logger_log(int level, const char *format, ...);

/**
 * @brief Cleans up and releases the resources associated with the logger.
 *
 * This function ensures that all resources held by the global logger are properly
 * deallocated. It closes the log file if it is open, resets the associated file pointer
 * to NULL, and destroys the mutex used for thread synchronization. It provides a safe
 * and consistent way to clean up the logging system before application termination.
 */
void logger_destroy(void);

#endif