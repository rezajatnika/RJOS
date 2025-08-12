#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "src/scheduler.h"
#include "src/system.h"
#include "src/udp.h"

void ex_logger(size_t index, void *data) {
    printf("[%d] Task %zu executed. Data: %p\n", millis(), index, data);
}

void ex_task(void *data) {
    for (int i = 0; i < 100; i++) {
        __asm__ __volatile__("nop");
    }
}

void log_task_stats(void *arg) {
    sched_t *sched = (sched_t *)arg;
    for (int i = 0; i < sched->tasks_count; i++) {
        sched_task_t *t = &sched->tasks[i];
        printf("[Task %s] runs: %u, avg: %d, max: %ums.\n", t->name, t->run_count, sched_avg_ms(t), t->max_duration_ms);
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

    sched_add_task(&sched, ex_task, "1234", 1000, 255 ,"ex_task");
    sched_add_task(&sched, ex_udp, &udp, 100, 255, "ex_udp");
    sched_add_task(&sched, log_task_stats, &sched, 1000, 128, "log_task_stats");
    sched_set_log_hook(&sched, NULL);

    sched_setup_signal_handlers();
    sched_start(&sched);

    printf("[%d] Shutdown requested at: %dms after start.\n", millis(), millis());
    sched_destroy(&sched);
    udp_close(&udp);
    return 0;
}