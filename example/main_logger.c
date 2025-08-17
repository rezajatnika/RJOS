#include "logger.h"

int main(void) {
    if (logger_init("log.txt", LOG_LEVEL_DEBUG) != 0) {
        printf("logger_init failed\n");
        return -1;
    }

    logger_log(LOG_LEVEL_DEBUG, "This is a DEBUG message.");
    logger_log(LOG_LEVEL_INFO,  "This is an INFO message.");
    logger_log(LOG_LEVEL_WARN,  "This is a WARN message.");

    logger_destroy();
    return 0;
}
