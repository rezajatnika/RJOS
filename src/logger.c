#include "logger.h"

#include <stdarg.h>
#include <string.h>

/**
 * @brief A static global logger instance used for logging purposes throughout the application.
 *
 * The global logger is initialized with a mutex to support thread-safe operations
 * on the logger and can be used as a shared resource for logging in a multithreaded
 * application.
 */
static logger_t glog = { NULL, LOG_LEVEL_DEBUG, 1, PTHREAD_MUTEX_INITIALIZER };

int logger_init(char *filename, int log_level) {
    pthread_mutex_lock(&glog.mutex);

    /* Close any existing file if logger is already initialized. */
    if (glog.file) {
        fclose(glog.file);
    }

    glog.file = fopen(filename, "a");
    if (!glog.file) {
        perror("logger_init: fopen");
        pthread_mutex_unlock(&glog.mutex);
        return -1;
    }
    glog.log_level = log_level;
    glog.enabled   = 1;
    pthread_mutex_unlock(&glog.mutex);

    return 0;
}

void logger_set_log_level(int log_level) {
    pthread_mutex_lock(&glog.mutex);
    glog.log_level = log_level;
    pthread_mutex_unlock(&glog.mutex);
}

void logger_enable(int enabled) {
    pthread_mutex_lock(&glog.mutex);
    glog.enabled = enabled;
    pthread_mutex_unlock(&glog.mutex);
}

void logger_log(int level, const char *format, ...) {
    if (level < glog.log_level || !glog.enabled) {
        return;
    }
    pthread_mutex_lock(&glog.mutex);

    /* Add a timestamp to the log. */
    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);
    char time_str[32];
    if (local_time) {
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", local_time);
    } else {
        strncpy(time_str, "UNKNOWN TIME", sizeof(time_str));
    }

    /* Write log message. */
    if (glog.file) {
        fprintf(glog.file, "[%s] ", time_str);
        switch (level) {
            case LOG_LEVEL_DEBUG:
                fprintf(glog.file, "DEBUG: ");
                break;
            case LOG_LEVEL_INFO:
                fprintf(glog.file, "INFO: ");
                break;
            case LOG_LEVEL_WARN:
                fprintf(glog.file, "WARN: ");
                break;
            case LOG_LEVEL_ERROR:
                fprintf(glog.file, "ERROR: ");
                break;
            default:
                fprintf(glog.file, "UNKNOWN: ");
                break;
        }
        /* Format the user-provided message. */
        va_list args;
        va_start(args, format);
        vfprintf(glog.file, format, args);
        va_end(args);

        /* Ensure logs are immediately written to the file. */
        fprintf(glog.file, "\n");
        fflush(glog.file);
    }
    pthread_mutex_unlock(&glog.mutex);
}

void logger_destroy(void) {
    pthread_mutex_lock(&glog.mutex);
    if (glog.file) {
        fclose(glog.file);
        glog.file = NULL;
    }
    pthread_mutex_unlock(&glog.mutex);
    pthread_mutex_destroy(&glog.mutex);
}

