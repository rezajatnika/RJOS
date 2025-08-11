#include <stdio.h>
#include "src/scheduler.h"

void print_hello(void *data) {
    printf("[%d] Hello every %s ms.\n", millis(), (char*)data);
}

int main(void) {
    sched_t sched;
    sched_init(&sched, 4);
    sched_add_task(&sched, print_hello, "1000", 1000);

    sched_setup_signal_handlers();
    sched_start(&sched);

    printf("[%d] Shutdown requested at: %dms after start.\n", millis(), millis());
    sched_destroy(&sched);
    return 0;
}