#ifndef RJOS_H
#define RJOS_H

#include "config.h"
#include "logger.h"
#include "rjos.h"
#include "system.h"

/**
 * @brief Initializes the system's runtime environment, configuration, and logging modules.
 *
 * This function initializes the core runtime components for the system, including
 * - System-level initialization handled by `system_init`.
 * - Loading the system configuration using the provided configuration file.
 * - Setting up the logging system with the specified log file and default debug log level.
 *
 * If any of the setup steps (configuration loading or logger initialization) fail,
 * the function will terminate the program with an error code.
 *
 * @param config_file Path to the configuration file that contains system settings.
 * @param log_file Path to the log file where logs will be written.
 */
void rjos_init(const char *config_file, char *log_file);

/**
 * @brief Cleans up and releases system resources used by the runtime environment.
 *
 * This function is responsible for finalizing the runtime system by:
 * - Destroying the configuration system via `config_destroy`.
 * - Shutting down and releasing resources allocated by the logging system via `logger_destroy`.
 *
 * This ensures that all system components are appropriately cleaned up before program termination.
 */
void rjos_cleanup(void);

#endif