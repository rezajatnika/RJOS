#include <stdio.h>
#include <string.h>

#include "src/scheduler.h"
#include "src/system.h"
#include "src/udp.h"

void ex_logger(size_t index, void *data) {
    printf("[%d] Task %zu executed. Data: %p\n", millis(), index, data);
}

void ex_task(void *data) {
    for (int i = 0; i < 10; i++) {
        __asm__ __volatile__("nop");
    }
}

void ex_udp(void *data) {
    udp_t *udp = data;
    char buffer[64];
    sprintf(buffer, "Hello from UDP: %d\n", millis());
    udp_send(udp, buffer, strlen(buffer));
}

int main(void) {
    sched_t sched;
    udp_t udp;
    sched_init(&sched, 4);
    udp_init(&udp, "127.0.0.1", 8000);

    sched_add_task(&sched, ex_task, "1234", 1000, 255);
    sched_add_task(&sched, ex_udp, &udp, 100, 255);
    sched_set_log_hook(&sched, NULL);

    sched_setup_signal_handlers();
    sched_start(&sched);

    printf("[%d] Shutdown requested at: %dms after start.\n", millis(), millis());
    sched_destroy(&sched);
    udp_close(&udp);
    return 0;
}