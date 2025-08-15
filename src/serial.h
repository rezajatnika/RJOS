#ifndef RJOS_SERIAL_H
#define RJOS_SERIAL_H

#include <stdint.h>
#include <sys/types.h>

/**
 * @enum serial_parity
 * @brief Defines the parity modes for serial communication.
 *
 * This enumeration represents the possible parity configurations
 * that can be used in serial communication. Parity is a form
 * of error checking used in data transmission.
 */
typedef enum serial_parity {
    SERIAL_PARITY_NONE = 0,
    SERIAL_PARITY_EVEN = 1,
    SERIAL_PARITY_ODD  = 2,
} serial_parity_t;

/**
 * @enum serial_flow
 * @brief Specifies the flow control methods for serial communication.
 *
 * This enumeration defines the different flow control mechanisms
 * that can be applied in serial communication to manage data flow.
 * Flow control ensures that data is transmitted and received
 * without overloading the communication channel.
 */
typedef enum serial_flow {
    SERIAL_FLOW_NONE    = 0,
    SERIAL_FLOW_RTSCTS  = 1,
    SERIAL_FLOW_XONXOFF = 2,
} serial_flow_t;

/**
 * @struct serial
 * @brief Represents a serial communication configuration and state.
 *
 * This structure is used to encapsulate the parameters and state
 * required for serial communication. It provides fields for configuration
 * options such as baud rate, data bits, stop bits, parity, and flow control,
 * as well as the file descriptor for the device, timeout settings, and
 * whether communication should be blocking or non-blocking.
 */
typedef struct serial {
    int             fd;
    char           *device;
    uint32_t        baudrate;
    int             data_bits;
    int             stop_bits;
    serial_parity_t parity;
    serial_flow_t   flow;
    int             blocking;
    int             timeout_ms;
} serial_t;

/**
 * Initialize and open a serial port.
 *
 * Blocking behavior:
 * - If blocking != 0, reads are blocking with VMIN/VTIME tuned by timeout_ms:
 *   - timeout_ms <= 0: indefinite blocking (VMIN=1, VTIME=0).
 *   - timeout_ms > 0 : return when any data arrives, else timeout (VMIN=0, VTIME=timeout_ms/100).
 * - If blocking == 0, fd is set O_NONBLOCK and timeout_ms is ignored.
 *
 * @param serial Pointer to serial_t to initialize (must not be NULL).
 * @param device Serial device path (e.g., "/dev/ttyUSB0", "/dev/tty.usbserial-XXXX").
 * @param baudrate Baud rate (e.g., 9600, 115200). Must map to supported speed.
 * @return 0 on success, -1 on failure (errno set).
 */
int serial_open(serial_t *serial, const char *device, uint32_t baudrate);

/**
 * @brief Sets the timeout duration for serial communication.
 *
 * This function configures the timeout for read/write operations
 * on the specified serial connection. The timeout is measured
 * in milliseconds.
 *
 * @param serial A pointer to the serial_t struct that represents
 *        the serial connection to configure.
 * @param timeout_ms The timeout duration in milliseconds. A value
 *        of 0 means no timeout will be enforced, and operations
 *        may block indefinitely.
 * @return 0 on success, or a negative value on failure to indicate
 *         an error setting the timeout.
 */
int serial_set_timeout(serial_t *serial, uint32_t timeout_ms);

/**
 * @brief Configures the blocking mode of a serial connection.
 *
 * Sets whether the serial communication should operate in blocking or
 * non-blocking mode.
 *
 * @param serial A pointer to the serial_t structure representing the serial connection.
 * @param blocking An integer indicating the desired mode: 1 for blocking mode, 0 for non-blocking mode.
 * @return 0 on success, or a negative value on failure.
 */
int serial_set_blocking(serial_t *serial, int blocking);

/**
 * @brief Writes data to the specified serial communication interface.
 *
 * This function transmits a defined number of bytes from a buffer through a given
 * serial interface. It requires a valid and initialized serial_t instance and
 * ensures that data is sent based on the configuration of the serial device.
 *
 * @param serial A pointer to the serial_t structure representing the serial communication interface.
 * @param buf A pointer to the buffer containing the data to be transmitted.
 * @param len The number of bytes to write from the buffer.
 * @return The number of bytes actually written, or -1 in case of an error.
 */
ssize_t serial_write(serial_t *serial, const void *buf, size_t len);

/**
 * @brief Reads data from a serial port.
 *
 * This function reads a specified number of bytes from a serial port into
 * a provided buffer. The actual number of bytes read may be less than the
 * requested length depending on the data available in the serial input buffer.
 *
 * @param serial A pointer to the serial_t structure representing the serial port.
 * @param buf A pointer to the buffer where the data will be written.
 * @param len The maximum number of bytes to read into the buffer.
 * @return The number of bytes actually read on success, or -1 on failure.
 */
ssize_t serial_read(serial_t *serial, void *buf, size_t len);

/**
 * @brief Flushes the input or output buffers of a serial connection.
 *
 * This function clears the specified input or output buffers
 * of the given serial connection. Flushing can either discard
 * any buffered data or wait for pending data transfers to complete,
 * depending on the provided parameters.
 *
 * @param serial A pointer to the serial_t struct representing the serial connection.
 * @param which  Indicates which buffer(s) to flush. For example, input, output, or both.
 * @return 0 on success, or a negative value on error.
 */
int serial_flush(serial_t *serial, int which);

/**
 * @brief Checks the number of bytes available to be read from the serial port.
 *
 * This function queries the serial port to determine how many bytes are currently
 * available in the input buffer and waiting to be read. The count is stored in
 * the location pointed to by out_count.
 *
 * @param serial A pointer to the serial_t structure representing the serial port.
 * @param out_count A pointer to a size_t variable where the number of available
 *        bytes will be stored.
 * @return 0 on success, or -1 on failure (with errno set appropriately).
 */
int serial_bytes_available(serial_t *serial, size_t *out_count);

/**
 * @brief Closes a serial port connection and releases associated resources.
 *
 * This function closes the file descriptor associated with the serial port
 * and frees any allocated memory. It should be called when the serial
 * communication is no longer needed to ensure proper cleanup of system
 * resources.
 *
 * @param serial Pointer to the serial_t structure representing the serial
 *        connection to be closed.
 * @return 0 on successful closure, -1 on failure (with errno set).
 */
int serial_close(serial_t *serial);

#endif
