#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/logger.h"
#include "../src/config.h"
#include "../src/scheduler.h"
#include "../src/system.h"
#include "../src/udp.h"

void send_message(void *arg) {
    udp_t *udp = (udp_t *)arg;
    char buf[256];
    snprintf(buf, sizeof(buf), "Hello from UDP: %d\n", millis());
    udp_send(udp, buf, strlen(buf));
}

int main(void) {
    /* System initialization. */
    system_init();
    logger_init("log.txt", LOG_LEVEL_DEBUG);

    /* Scheduler initialization. */
    sched_t sched;
    sched_init(&sched, 4);

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

    sched_add_task(&sched, send_message, &udp, 10, 0, "send_message");
    sched_set_log_hook(&sched, NULL);
    sched_setup_signal_handlers();
    sched_start(&sched);

    config_destroy(&config);
    udp_close(&udp);
    sched_destroy(&sched);
    return 0;
}