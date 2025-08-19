#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "rjos.h"
#include "serial.h"

int main(void) {
    rjos_init("config.txt", "log.txt");

    const char *serial_device = config_get("serial_device");
    const char *baudrate      = config_get("baudrate");

    serial_t serial;
    serial_open(&serial, serial_device, atoi(baudrate));

    char buf[256];
    snprintf(buf, sizeof(buf), "%s: %d ms\n", serial.device, millis());
    for (int i = 0; i < 10; ++i) {
        serial_write(&serial, buf, strlen(buf));
    }
    serial_close(&serial);

    rjos_cleanup();
    return 0;
}