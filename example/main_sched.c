#include <stdio.h>

#include "../src/config.h"
#include "../src/scheduler.h"
#include "../src/system.h"

void task_1hz() {
    static int count = 0;
    printf("Task 1hz: %d %d\n", ++count, millis());
}

void task_2hz() {
    static int count = 0;
    printf("Task 2hz: %d %d\n", ++count, millis());
}

int main(void) {
    /* System initialization. */
    system_init();

    /* Configuration initialization. */
    config_t config;
    config_init(&config);

    /* Scheduler initialization. */
    sched_t sched;
    sched_init(&sched, 4);
    sched_add_task(&sched, task_1hz, NULL, 1000, 0, "task_1hz");
    sched_add_task(&sched, task_2hz, NULL, 2000, 0, "task_2hz");

    sched_set_log_hook(&sched, NULL);
    sched_setup_signal_handlers();
    sched_start(&sched);

    sched_destroy(&sched);
    return 0;
}