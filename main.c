#include <stdio.h>
#include "src/scheduler.h"

void ex_logger(size_t index, void *data) {
    printf("[RJOS] Task %zu executed. Data: %p\n", index, data);
}

void ex_task(void *data) {
    for (int i = 0; i < 10; i++) {
        __asm__ __volatile__("nop");
    }
}

int main(void) {
    FILE *logf = fopen("log.txt", "w");
    fclose(logf);

    sched_t sched;
    sched_init(&sched, 4);
    sched_add_task(&sched, ex_task, NULL, 1000);
    sched_set_log_hook(&sched, ex_logger);

    sched_setup_signal_handlers();
    sched_start(&sched);

    printf("[%d] Shutdown requested at: %dms after start.\n", millis(), millis());
    fclose(logf);
    sched_destroy(&sched);
    return 0;
}