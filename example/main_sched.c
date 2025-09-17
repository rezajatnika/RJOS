#include <stdio.h>

#include "pelco_d.h"
#include "rjos.h"
#include "scheduler.h"

pd_msg_t pmsg;

void task_1hz1(void *args) {
    pd_query_pos(&pmsg, 0x01, PELCO_D_PAN);
    pd_print_msg(&pmsg);
}

void task_1hz2(void *args) {
    pd_query_pos(&pmsg, 0x01, PELCO_D_TILT);
    pd_print_msg(&pmsg);
}

void task_1hz3(void *args) {
    uint8_t buff[7];
    pd_set_pan(&pmsg, 0x01, 9000);
    pd_pack_message(&pmsg, buff, 7);
    pd_print_bytes(buff, 7);
}

int main(void) {
    rjos_init("config.txt", "log.txt");

    /* Scheduler initialization. */
    sched_init(4);

    /* Add tasks to the scheduler. */
    //sched_add_task(task_1hz1, NULL, 1000, 1, "task_1hz");
    //sched_add_task(task_1hz2, NULL, 1000, 2, "task_1hz");
    sched_add_task(task_1hz3, NULL, 1000, 3, "task_1hz");

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