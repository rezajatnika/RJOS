#include "rjos.h"

int main(void) {
    rjos_init("config.txt", "log.txt");

    /* Example logging a message. */
    logger_log(LOG_LEVEL_DEBUG, "This is a debug message.");

    rjos_cleanup();
    return 0;
}