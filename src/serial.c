#include "serial.h"

#include <fcntl.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

/**
 * Sets the FD_CLOEXEC flag on the specified file descriptor.
 *
 * This method configures the file descriptor to be automatically closed
 * when executing a new program using exec-family system calls.
 *
 * @param fd The file descriptor to modify.
 * @return 0 on success, -1 on failure, and sets errno appropriately.
 */
static int set_cloexec(int fd) {
    int flags = fcntl(fd, F_GETFD);
    if (flags < 0) {
        perror("set_cloexec: fcntl");
        return -1;
    }
    return fcntl(fd, F_SETFD, flags | FD_CLOEXEC);
}

static int set_nonblocking_flag(int fd, int enable) {
    int flags = fcntl(fd, F_GETFL);
    if (flags < 0) {
        perror("set_nonblocking_flag: fcntl");
        return -1;
    }
    if (enable) {
        flags |= O_NONBLOCK;
    } else {
        flags &= ~O_NONBLOCK;
    }
    return fcntl(fd, F_SETFL, flags);
}

/**
 * Maps a standard baud rate value to its corresponding speed_t constant.
 *
 * This function translates a baud rate specified as a uint32_t value
 * into the appropriate speed_t value used by termios configuration.
 * If the provided baud rate is not supported, the function returns 0.
 *
 * @param baud The baud rate to convert (e.g., 9600, 19200, 115200).
 * @return The termios speed_t constant for the baud rate, or 0 if the baud rate is unsupported.
 */
static speed_t map_baud(uint32_t baud) {
    switch (baud) {
        case 9600:
            return B9600;
        case 19200:
            return B19200;
        case 38400:
            return B38400;
        case 57600:
            return B57600;
        case 115200:
            return B115200;
        default:
            return 0;
    }
}

/**
 * Configures terminal I/O settings for a serial port.
 *
 * This function applies various terminal I/O configurations, such as
 * baud rate, data bits, stop bits, parity, and flow control. It also
 * handles blocking or non-blocking modes based on the provided parameters.
 *
 * @param fd The file descriptor of the serial port.
 * @param serial A pointer to a serial_t structure containing the desired
 *        configuration parameters, including baudrate, data bits,
 *        stop bits, parity settings, flow control, and timeout.
 * @return 0 on success, -1 on failure, and sets errno appropriately.
 */
static int apply_termios(int fd, serial_t *serial) {
    struct termios tio;
    if (tcgetattr(fd, &tio) < 0) {
        perror("apply_termios: tcgetattr");
        return -1;
    }

    /* Start from "raw then add the features we need. */
    cfmakeraw(&tio);

    /* Baud rate configuration. */
    speed_t speed = map_baud(serial->baudrate);
    if (speed == 0) {
        perror("apply_termios: map_baud");
        return -1;
    }
    if (cfsetispeed(&tio, speed) < 0) return -1;
    if (cfsetospeed(&tio, speed) < 0) return -1;

    /* Data bits configuration. */
    tio.c_cflag &= ~CSIZE;
    switch (serial->data_bits) {
        case 5: tio.c_cflag |= CS5; break;
        case 6: tio.c_cflag |= CS6; break;
        case 7: tio.c_cflag |= CS7; break;
        case 8: tio.c_cflag |= CS8; break;
        default:
            return -1;
    }

    /* Stop bits configuration. */
    switch (serial->stop_bits) {
        case 1: tio.c_cflag &= ~CSTOPB; break;
        case 2: tio.c_cflag |=  CSTOPB; break;
        default:
            return -1;
    }

    /* Parity configuration. */
    tio.c_cflag &= ~(PARENB | PARODD);
    if (serial->parity == SERIAL_PARITY_EVEN) {
        tio.c_cflag |=  PARENB;
        tio.c_cflag &= ~PARODD;
    } else if (serial->parity == SERIAL_PARITY_ODD) {
        tio.c_cflag |= PARENB;
        tio.c_cflag |= PARODD;
    }

    /* Flow control configuration, disable all first. */
#ifdef CRTSCTS
    tio.c_cflag &= ~CRTSCTS;
#elif defined(CRTS_IFLOW) && defined(CTS_OFLOW)
    tio.c_cflag &= ~(CRTS_IFLOW | CTS_OFLOW);
#endif
    tio.c_iflag &= ~(IXON | IXOFF | IXANY);

    if (serial->flow == SERIAL_FLOW_RTSCTS) {
#ifdef CRTSCTS
        tio.c_cflag |= CRTSCTS;
#elif defined(CRTS_IFLOW) && defined(CTS_OFLOW)
        tio.c_cflag |= (CRTS_IFLOW | CCTS_OFLOW);
#else
        return -1;
#endif
    } else if (serial->flow == SERIAL_FLOW_XONXOFF) {
        tio.c_iflag |= (IXON | IXOFF | IXANY);
    }

    /* Make sure we read data and ignore modem control lines. */
    tio.c_cflag |= (CLOCAL | CREAD);

    if (serial->blocking) {
        if (serial->timeout_ms <= 0) {
            tio.c_cc[VMIN]  = 1;
            tio.c_cc[VTIME] = 0;
        } else {
            int ds = serial->timeout_ms / 100;
            if (ds <=   0) ds = 1;
            if (ds <= 255) ds = 255;
            tio.c_cc[VMIN]  = 0;
            tio.c_cc[VTIME] = ds;
        }
    } else {
        tio.c_cc[VMIN]  = 0;
        tio.c_cc[VTIME] = 0;
    }
    if (tcsetattr(fd, TCSANOW, &tio) < 0) {
        perror("apply_termios: tcsetattr");
        return -1;
    }
    tcflush(fd, TCIFLUSH);
    return 0;
}

int serial_open(serial_t *serial, const char *device, uint32_t baudrate) {
    if (!serial || !device) {
        perror("serial_open");
        return -1;
    }
    serial->fd = -1;
    serial->device = NULL;
    serial->baudrate = 0;
    serial->data_bits = 0;
    serial->stop_bits = 0;
    serial->parity = SERIAL_PARITY_NONE;
    serial->flow = SERIAL_FLOW_NONE;
    serial->blocking = 1;
    serial->timeout_ms = 0;

    int fd = open(device, O_RDWR | O_NOCTTY);
    if (fd < 0) {
        return -1;
    }
    set_cloexec(fd);

    if (!serial->blocking) {
        if (set_nonblocking_flag(fd, 1) < 0) {
            close(fd);
            return -1;
        }
    }
    char *dup = strdup(device);
    if (!dup) {
        return -1;
    }
    serial->fd = fd;
    serial->device = dup;
    serial->baudrate = baudrate;
    serial->data_bits = 8;
    serial->stop_bits = 1;
    serial->parity = SERIAL_PARITY_NONE;
    serial->flow = SERIAL_FLOW_NONE;
    serial->blocking = 1;
    serial->timeout_ms = 1000;
    return 0;
}

int serial_set_timeout(serial_t *serial, uint32_t timeout_ms) {
    if (!serial || serial->fd < 0) {
        perror("serial_set_timeout");
        return -1;
    }
    if (!serial->blocking) {
        serial->timeout_ms = 0;
        return 0;
    }
    serial->timeout_ms = timeout_ms;
    return apply_termios(serial->fd, serial);
}

int serial_set_blocking(serial_t *serial, int blocking) {
    if (!serial || serial->fd < 0) {
        perror("serial_set_blocking");
        return -1;
    }
    if (set_nonblocking_flag(serial->fd, blocking) < 0) {
        return -1;
    }
    serial->blocking = blocking;
    return apply_termios(serial->fd, serial);
}

ssize_t serial_write(serial_t *serial, const void *buf, size_t len) {
    if (!serial || serial->fd < 0 || (!buf && len) > 0) {
        perror("serial_write");
        return -1;
    }
    if (len == 0) {
        return 0;
    }
    const uint8_t *p = (const uint8_t *)buf;

    int total = 0;
    for (;;) {
        ssize_t n = write(serial->fd, p + total, len - total);
        if (n >= 0) {
            total += (size_t)n;
            if (total == len || !serial->blocking) {
                return total;
            }
            continue;
        }
        return -1;
    }
}

ssize_t serial_read(serial_t *serial, void *buf, size_t len) {
    if (!serial || serial->fd < 0 || (!buf && len) > 0) {
        perror("serial_read");
        return -1;
    }
    if (len == 0) return 0;
    for (;;) {
        ssize_t n = read(serial->fd, buf, len);
        if (n >-0) {
            return n;
        }
        return -1;
    }
}

int serial_bytes_available(serial_t *serial, size_t *out_count) {
    if (!serial || serial->fd < 0) {
        perror("serial_bytes_available");
        return -1;
    }
    int n = 0;
    if (ioctl(serial->fd, FIONREAD, &n) < 0) {
        perror("serial_bytes_available: ioctl");
        return -1;
    }
    if (n < 0) n = 0;
    *out_count = (size_t)n;
    return 0;
}

int serial_close(serial_t *serial) {
    if (serial && serial->fd != -1) {
        close(serial->fd);
        serial->fd = -1;
        serial->device = NULL;
        serial->baudrate = 0;
        serial->data_bits = 0;
        serial->stop_bits = 0;
        serial->parity = SERIAL_PARITY_NONE;
        serial->flow = SERIAL_FLOW_NONE;
        serial->blocking = 1;
        serial->timeout_ms = 0;
        return 0;
    }
    return -1;
}
