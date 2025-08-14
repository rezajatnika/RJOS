#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "src/scheduler.h"
#include "src/serial.h"
#include "src/system.h"
#include "src/udp.h"

void ex_udp(void *data) {
    udp_t *udp = (udp_t *)data;
    char buf[128];
    snprintf(buf, sizeof(buf), "[%s:%d]: millis: %d\n", udp->host, udp->port, millis());
    udp_send(udp, buf, strlen(buf));
}

void ex_serial(void *data) {
    serial_t *serial = (serial_t *)data;
    char buf[128];
    snprintf(buf, sizeof(buf), "[%s]: millis: %d\n", serial->device, millis());
    serial_write(serial, buf, strlen(buf));
}

int main(void) {
    system_init();

    sched_t sched;
    sched_init(&sched, 4);

    udp_t udp;
    udp_init(&udp, "127.0.0.1", 8000);

    serial_t serial;
    serial_open(&serial, "/dev/ttys002", 115200);
    serial_set_blocking(&serial, 1);
    serial_set_timeout(&serial, 1000);

    sched_add_task(&sched, ex_udp, &udp, 1000, 0 ,"ex_udp");
    sched_add_task(&sched, ex_serial, &serial, 1000, 1, "ex_serial");
    sched_set_log_hook(&sched, NULL);

    sched_setup_signal_handlers();
    sched_start(&sched);

    printf("[%d] Shutdown requested at: %dms after start.\n", millis(), millis());
    sched_destroy(&sched);
    udp_close(&udp);
    serial_close(&serial);
    return 0;
}