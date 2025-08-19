#include <stdio.h>

#include "rjos.h"
#include "scheduler.h"

void task_1hz(void *args) {
    static int count = 0;
    printf("Task 1hz: %d %d\n", ++count, millis());
}

void task_2hz(void *args) {
    static int count = 0;
    printf("Task 2hz: %d %d\n", ++count, millis());
}

int main(void) {
    rjos_init("config.txt", "log.txt");

    /* Scheduler initialization. */
    sched_init(4);

    /* Add tasks to the scheduler. */
    sched_add_task(task_1hz, NULL, 1000, 0, "task_1hz");
    sched_add_task(task_2hz, NULL,  500, 1, "task_2hz");

    /* Setup scheduler log callback and signal handlers. */
    sched_set_log_hook(NULL);
    sched_setup_signal_handlers();

    /* Start the scheduler. */
    sched_start();

    /* Release scheduler. */
    sched_destroy();

    rjos_cleanup();
    return 0;
}