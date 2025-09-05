#include "rjos.h"
#include <stdlib.h>

void rjos_init(const char *config_file, char *log_file) {
    /* System initialization. */
    system_init();

    /* Configuration & Logger initialization. */
    if (config_load(config_file) != 0 || logger_init(log_file, LOG_LEVEL_DEBUG) != 0) {
        exit(EXIT_FAILURE);
    }
}

void rjos_cleanup(void) {
    config_destroy();
    logger_destroy();
}