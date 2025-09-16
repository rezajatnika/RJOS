#include <stdio.h>

#include "pelco_d.h"
#include "rjos.h"
#include "scheduler.h"

pelco_d_message_t pmsg;

void task_1hz1(void *args) {
    pelco_d_query_position(&pmsg, 0x01, PELCO_D_PAN);
    pelco_d_print_message(&pmsg);
}

void task_1hz2(void *args) {
    pelco_d_query_position(&pmsg, 0x01, PELCO_D_TILT);
    pelco_d_print_message(&pmsg);
}

void task_1hz3(void *args) {
    uint8_t buff[7];
    pelco_d_create_message(&pmsg, 0x01, 0x00, 0x00, 0x00, 0x00);
    pelco_d_message_to_bytes(&pmsg, buff, 7);
    pelco_d_print_bytes(buff, 7);
}

int main(void) {
    rjos_init("config.txt", "log.txt");

    /* Scheduler initialization. */
    sched_init(4);

    /* Add tasks to the scheduler. */
    sched_add_task(task_1hz1, NULL, 1000, 0, "task_1hz");
    sched_add_task(task_1hz2, NULL, 1000, 0, "task_1hz");
    sched_add_task(task_1hz3, NULL, 1000, 0, "task_1hz");

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