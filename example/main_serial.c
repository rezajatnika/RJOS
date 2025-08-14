#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../src/config.h"
#include "../src/system.h"
#include "../src/serial.h"

int main(void) {
    /* System initialization. */
    system_init();

    /* Configuration initialization. */
    config_t config;
    config_init(&config);

    if (config_load(&config, "ex_config.txt") != 0) {
        return -1;
    }
    const char *serial_device = config_get(&config, "serial_device");
    const char *timeout_ms    = config_get(&config, "timeout_ms");
    const char *baudrate      = config_get(&config, "baudrate");
    const char *blocking      = config_get(&config, "blocking");

    serial_t serial;
    serial_open(&serial, serial_device, atoi(baudrate));

    char buf[256];
    snprintf(buf, sizeof(buf), "%s: %d ms\n", serial.device, millis());
    for (int i = 0; i < 10; ++i) {
        serial_write(&serial, buf, strlen(buf));
    }
    serial_close(&serial);
    return 0;
}