#include <stdio.h>
#include "../src/config.h"
#include "../src/system.h"

int main(void) {
    system_init();

    config_t config;
    config_init(&config);

    if (config_load(&config, "ex_config.txt") != 0) {
        return -1;
    }

    const char *host = config_get(&config, "host");
    const char *port = config_get(&config, "port");
    const char *mode = config_get(&config, "mode");

    printf("[%d ms] UDP Host: %s\n", millis(), host ? host : "not set");
    printf("[%d ms] UDP Port: %s\n", millis(), port ? port : "not set");
    printf("[%d ms] UDP Mode: %s\n", millis(), mode ? mode : "not set");

    config_destroy(&config);
    return 0;
}
