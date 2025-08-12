#include <stdio.h>
#include "src/scheduler.h"
#include "src/system.h"

void ex_logger(size_t index, void *data) {
    printf("[%d] Task %zu executed. Data: %p\n", millis(), index, data);
}

void ex_task(void *data) {
    for (int i = 0; i < 10; i++) {
        __asm__ __volatile__("nop");
    }
}

int main(void) {
    sched_t sched;
    sched_init(&sched, 4);
    sched_add_task(&sched, ex_task, "1234", 1000, 255);
    sched_add_task(&sched, ex_task, "3456", 1000, 0);
    sched_add_task(&sched, ex_task, "7890", 1000, 128);
    sched_set_log_hook(&sched, ex_logger);

    sched_setup_signal_handlers();
    sched_start(&sched);

    printf("[%d] Shutdown requested at: %dms after start.\n", millis(), millis());
    sched_destroy(&sched);
    return 0;
}