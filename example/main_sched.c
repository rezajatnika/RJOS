#include <stdio.h>

#include "config.h"
#include "scheduler.h"
#include "system.h"
#include "logger.h"

void task_1hz(void *args) {
    static int count = 0;
    printf("Task 1hz: %d %d\n", ++count, millis());
}

void task_2hz(void *args) {
    static int count = 0;
    printf("Task 2hz: %d %d\n", ++count, millis());
}

int main(void) {
    /* System initialization. */
    system_init();
    logger_init("log.txt", LOG_LEVEL_DEBUG);

    /* Configuration initialization. */
    config_load("config.txt");

    /* Scheduler initialization. */
    sched_init(4);

    /* Add tasks to the scheduler. */
    sched_add_task(task_1hz, NULL, 1000, 0, "task_1hz");
    sched_add_task(task_2hz, NULL,  500, 1, "task_2hz");

    sched_set_log_hook(NULL);
    sched_setup_signal_handlers();
    sched_start();

    config_destroy();
    sched_destroy();
    logger_destroy();
    return 0;
}