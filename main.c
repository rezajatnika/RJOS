#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "src/scheduler.h"
#include "src/system.h"
#include "src/udp.h"

void ex_task(void *data) {
    udp_t *udp = (udp_t *)data;
    char buf[128];
    snprintf(buf, sizeof(buf), "%s, %d\n", udp->host, millis());
    udp_send(udp, buf, strlen(buf));
}

int main(void) {
    sched_t sched;
    udp_t udp;
    system_init();
    sched_init(&sched, 4);
    udp_init(&udp, "127.0.0.1", 8000);

    sched_add_task(&sched, ex_task, &udp, 1000, 0 ,"ex_task");
    sched_set_log_hook(&sched, NULL);

    sched_setup_signal_handlers();
    sched_start(&sched);

    printf("[%d] Shutdown requested at: %dms after start.\n", millis(), millis());
    sched_destroy(&sched);
    udp_close(&udp);
    return 0;
}