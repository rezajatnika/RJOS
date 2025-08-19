#include "../src/config.h"
#include "../src/system.h"
#include "../src/logger.h"

#include <stdio.h>

int main(void) {
    system_init();
    logger_init("log.txt", LOG_LEVEL_DEBUG);

    if (config_load("config.txt") != 0) {
        return -1;
    }

    const char *host = config_get("host");
    const char *port = config_get("port");
    const char *mode = config_get("mode");

    printf("[%d ms] UDP Host: %s\n", millis(), host ? host : "not set");
    printf("[%d ms] UDP Port: %s\n", millis(), port ? port : "not set");
    printf("[%d ms] UDP Mode: %s\n", millis(), mode ? mode : "not set");

    config_destroy();
    logger_destroy();
    return 0;
}
