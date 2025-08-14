#include <stdlib.h>

#include "src/config.h"
#include "src/system.h"
#include "src/udp.h"

int main(void) {
    /* System initialization. */
    system_init();

    /* Configuration initialization. */
    config_t config;
    config_init(&config);

    if (config_load(&config, "ex_config.txt") != 0) {
        return -1;
    }
    const char *host = config_get(&config, "host");
    const char *port = config_get(&config, "port");

    udp_t udp;
    udp_init(&udp, host, atoi(port));
    for (int i = 0; i < 10; ++i)
        udp_send(&udp, "Hello, world!\n", 14);

    udp_close(&udp);
    return 0;
}