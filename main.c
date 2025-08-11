#include <stdio.h>
#include "src/scheduler.h"

void print_hello(void *data) {
    printf("[%d] Hello every %sms.\n", millis(), (char*)data);
}

void log_hello(void *data) {
    FILE *log_file = (FILE*)data;
    log_file = fopen("log.txt", "a");
    fprintf(log_file, "[%d] Hello every %sms.\n", millis(), "1000");
}

int main(void) {
    FILE *logf = fopen("log.txt", "w");
    fclose(logf);

    sched_t sched;
    sched_init(&sched, 4);
    sched_add_task(&sched, print_hello, "1000", 1000);
    sched_add_task(&sched, log_hello, logf, 1000);

    sched_setup_signal_handlers();
    sched_start(&sched);

    printf("[%d] Shutdown requested at: %dms after start.\n", millis(), millis());
    fclose(logf);
    sched_destroy(&sched);
    return 0;
}