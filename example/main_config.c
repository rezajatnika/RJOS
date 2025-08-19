#include "rjos.h"
#include <stdio.h>

int main(void) {
    rjos_init("config.txt", "log.txt");

    const char *host = config_get("host");
    const char *port = config_get("port");
    const char *mode = config_get("mode");

    printf("[%d ms] UDP Host: %s\n", millis(), host ? host : "not set");
    printf("[%d ms] UDP Port: %s\n", millis(), port ? port : "not set");
    printf("[%d ms] UDP Mode: %s\n", millis(), mode ? mode : "not set");

    rjos_cleanup();
    return 0;
}