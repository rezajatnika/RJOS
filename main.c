#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "src/scheduler.h"
#include "src/serial.h"
#include "src/system.h"
#include "src/udp.h"

#define UDP_HOST "127.0.0.1"
#define UDP_PORT 8000
#define SERIAL_DEVICE "/dev/ttys002"
#define SERIAL_BAUDRATE 115200

// Function prototypes
void ex_udp(void *data);
void ex_serial(void *data);
void setup_udp(udp_t *udp);
void setup_serial(serial_t *serial);
void setup_scheduler(sched_t *sched, udp_t *udp, serial_t *serial);
void cleanup_resources(sched_t *sched, udp_t *udp, serial_t *serial);

// Task function for sending UDP datagrams
void ex_udp(void *data) {
    udp_t *udp = (udp_t *)data;
    char buf[128];
    snprintf(buf, sizeof(buf), "[%s:%d]: millis: %d\n", udp->host, udp->port, millis());
    udp_send(udp, buf, strlen(buf));
}

// Task function for serial communication
void ex_serial(void *data) {
    serial_t *serial = (serial_t *)data;
    char buf[128];
    snprintf(buf, sizeof(buf), "[%s]: millis: %d\n", serial->device, millis());
    serial_write(serial, buf, strlen(buf));
}

// Initialize and configure the UDP connection
void setup_udp(udp_t *udp) {
    if (udp_init(udp, UDP_HOST, UDP_PORT) != 0) {
        fprintf(stderr, "Failed to initialize UDP at %s:%d\n", UDP_HOST, UDP_PORT);
    }
}

// Initialize and configure the serial connection
void setup_serial(serial_t *serial) {
    if (serial_open(serial, SERIAL_DEVICE, SERIAL_BAUDRATE) != 0) {
        fprintf(stderr, "Failed to open serial device %s\n", SERIAL_DEVICE);
    }
    serial_set_blocking(serial, 1);
    serial_set_timeout(serial, 1000);
}

// Set up the scheduler tasks
void setup_scheduler(sched_t *sched, udp_t *udp, serial_t *serial) {
    if (sched_init(sched, 4) != 0) {
        fprintf(stderr, "Failed to initialize scheduler\n");
    }

    sched_add_task(sched, ex_udp, udp, 1000, 0, "ex_udp");
    sched_add_task(sched, ex_serial, serial, 1000, 1, "ex_serial");
    sched_set_log_hook(sched, NULL);
}

// Clean up and release resources
void cleanup_resources(sched_t *sched, udp_t *udp, serial_t *serial) {
    sched_destroy(sched);
    udp_close(udp);
    serial_close(serial);
}

// Main function
int main(void) {
    system_init();

    sched_t sched;
    udp_t udp;
    serial_t serial;

    // Initialize components
    setup_udp(&udp);
    setup_serial(&serial);
    setup_scheduler(&sched, &udp, &serial);

    // Set up signal handlers and start the scheduler
    sched_setup_signal_handlers();
    sched_start(&sched);

    // Print shutdown info and clean up
    printf("[%d] Shutdown requested at: %dms after start.\n", millis(), millis());
    cleanup_resources(&sched, &udp, &serial);

    return 0;
}