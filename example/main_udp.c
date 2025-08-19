#include <stdlib.h>

#include "rjos.h"
#include "udp.h"

int main(void) {
    rjos_init("config.txt", "log.txt");

    const char *host = config_get("host");
    const char *port = config_get("port");

    udp_t udp;
    udp_init(&udp, host, atoi(port));
    for (int i = 0; i < 10; ++i) {
        udp_send(&udp, "Hello from UDP", 14);
    }
    udp_close(&udp);

    rjos_cleanup();
    return 0;
}