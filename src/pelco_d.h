#ifndef PELCO_D_H
#define PELCO_D_H

#include <stdint.h>
#include <stdio.h>

#define PELCO_D_SYNC_BYTE           0xFF
#define PELCO_D_MESSAGE_SIZE           7
#define PELCO_D_MAX_ADDRESS          255
#define PELCO_D_MAX_SPEED           0x3F
#define PELCO_D_MAX_PRESET           255

#define PELCO_D_CMD1_SENSE          0x80
#define PELCO_D_CMD1_RESERVED1      0x40
#define PELCO_D_CMD1_RESERVED2      0x20
#define PELCO_D_CMD1_AUTO_SCAN      0x10
#define PELCO_D_CMD1_CAMERA_ON_OFF  0x08
#define PELCO_D_CMD1_IRIS_CLOSE     0x04
#define PELCO_D_CMD1_IRIS_OPEN      0x02
#define PELCO_D_CMD1_FOCUS_NEAR     0x01

#define PELCO_D_CMD2_FOCUS_FAR      0x80
#define PELCO_D_CMD2_ZOOM_WIDE      0x40
#define PELCO_D_CMD2_ZOOM_TELE      0x20
#define PELCO_D_CMD2_TILT_DOWN      0x10
#define PELCO_D_CMD2_TILT_UP        0x08
#define PELCO_D_CMD2_PAN_LEFT       0x04
#define PELCO_D_CMD2_PAN_RIGHT      0x02
#define PELCO_D_CMD2_RESERVED       0x01
#define PELCO_D_CMD2_Q_PAN_POS      0x51
#define PELCO_D_CMD2_S_PAN_POS      0x4B
#define PELCO_D_CMD2_Q_TILT_POS     0x53
#define PELCO_D_CMD2_S_TILT_POS     0x4D

#define PELCO_D_EXT_SET_PRESET      0x03
#define PELCO_D_EXT_CLEAR_PRESET    0x05
#define PELCO_D_EXT_GOTO_PRESET     0x07
#define PELCO_D_EXT_FLIP            0x20
#define PELCO_D_EXT_GOTO_ZERO_PAN   0x22
#define PELCO_D_EXT_SET_AUX         0x09
#define PELCO_D_EXT_CLEAR_AUX       0x0B
#define PELCO_D_EXT_REMOTE_RESET    0x0F
#define PELCO_D_EXT_ZONE_START      0x11
#define PELCO_D_EXT_ZONE_END        0x13
#define PELCO_D_EXT_WRITE_CHAR      0x15
#define PELCO_D_EXT_CLEAR_SCREEN    0x17
#define PELCO_D_EXT_ALARM_ACK       0x19
#define PELCO_D_EXT_ZONE_SCAN_START 0x1B
#define PELCO_D_EXT_ZONE_SCAN_STOP  0x1D
#define PELCO_D_EXT_PATTERN_START   0x1F
#define PELCO_D_EXT_PATTERN_STOP    0x21

typedef enum {
    PELCO_D_SUCCESS = 0,
    PELCO_D_ERROR_INVALID_ADDRESS,
    PELCO_D_ERROR_INVALID_SPEED,
    PELCO_D_ERROR_INVALID_PRESET,
    PELCO_D_ERROR_INVALID_AUX,
    PELCO_D_ERROR_NULL_POINTER,
    PELCO_D_ERROR_CHECKSUM,
} pelco_d_error_t;

typedef struct {
    uint8_t sync;
    uint8_t address;
    uint8_t command1;
    uint8_t command2;
    uint8_t data1;
    uint8_t data2;
    uint8_t checksum;
} pelco_d_message_t;

typedef enum {
    PELCO_D_PAN_STOP = 0,
    PELCO_D_PAN_LEFT,
    PELCO_D_PAN_RIGHT,
} pelco_d_pan_direction_t;

typedef enum {
    PELCO_D_TILT_STOP = 0,
    PELCO_D_TILT_UP,
    PELCO_D_TILT_DOWN,
} pelco_d_tilt_direction_t;

typedef enum {
    PELCO_D_ZOOM_STOP = 0,
    PELCO_D_ZOOM_WIDE,
    PELCO_D_ZOOM_TELE,
} pelco_d_zoom_direction_t;

typedef enum {
    PELCO_D_PAN,
    PELCO_D_TILT,
    PELCO_D_ZOOM
} pelco_d_query_pos_t;

/**
 * @brief Creates a Pelco-D protocol message with the specified parameters.
 *
 * This function initializes a Pelco-D message structure by populating it with
 * the given parameters: address, command1, command2, data1, data2, and calculates
 * the checksum for the generated message.
 *
 * @param[out] msg      Pointer to a `pelco_d_message_t` structure where the created
 *                      message will be stored. Must not be NULL.
 * @param[in] address   Device address for the Pelco-D message. Must be in the range
 *                      [0, PELCO_D_MAX_ADDRESS].
 * @param[in] command1  First command byte of the Pelco-D message.
 * @param[in] command2  Second command byte of the Pelco-D message.
 * @param[in] data1     First data byte for the Pelco-D message.
 * @param[in] data2     Second data byte for the Pelco-D message.
 *
 * @return Returns a `pelco_d_error_t` indicating the success or failure of
 * the operation:
 * - `PELCO_D_SUCCESS` if the message was created successfully.
 * - `PELCO_D_ERROR_NULL_POINTER` if the `msg` pointer is NULL.
 * - `PELCO_D_ERROR_INVALID_ADDRESS` if `address` exceeds `PELCO_D_MAX_ADDRESS`.
 */
pelco_d_error_t pelco_d_create_message(pelco_d_message_t *msg, uint8_t address, uint8_t command1, uint8_t command2, uint8_t data1, uint8_t data2);

/**
 * @brief Validates a Pelco-D protocol message.
 *
 * This function checks the validity of a given Pelco-D message by verifying
 * its structure, address, synchronization byte, and checksum. The provided
 * message is considered valid if:
 * - It has a valid synchronization byte.
 * - Its address is within the valid range.
 * - Its checksum matches the calculated checksum.
 *
 * @param[in] msg  Pointer to the `pelco_d_message_t` structure representing
 *                 the Pelco-D message to be validated. Must not be NULL.
 *
 * @return Returns a `pelco_d_error_t` indicating the result of the validation:
 * - `PELCO_D_SUCCESS` if the message is valid.
 * - `PELCO_D_ERROR_NULL_POINTER` if the `msg` pointer is NULL.
 * - `PELCO_D_ERROR_CHECKSUM` if the checksum or synchronization byte is invalid.
 * - `PELCO_D_ERROR_INVALID_ADDRESS` if the address exceeds `PELCO_D_MAX_ADDRESS`.
 */
pelco_d_error_t pelco_d_validate_message(const pelco_d_message_t *msg);

/**
 * @brief Constructs a Pelco-D pan-tilt message with specified parameters.
 *
 * This function generates a Pelco-D message to control pan and tilt operations
 * for a camera. Direction and speed for both pan and tilt movements are configured
 * based on the provided input parameters. The generated message includes the
 * address, command bytes, and speed values.
 *
 * @param[out] msg        Pointer to a `pelco_d_message_t` structure where the constructed
 *                        message will be stored. Must not be NULL.
 * @param[in] address     Device address for the Pelco-D message. Must be in the range
 *                        [0, PELCO_D_MAX_ADDRESS].
 * @param[in] pan_dir     Direction of pan movement. Can be one of the following:
 *                        `PELCO_D_PAN_STOP`, `PELCO_D_PAN_LEFT`, or `PELCO_D_PAN_RIGHT`.
 * @param[in] pan_speed   Speed of pan movement. Must be in the range [0, PELCO_D_MAX_SPEED].
 * @param[in] tilt_dir    Direction of tilt movement. Can be one of the following:
 *                        `PELCO_D_TILT_STOP`, `PELCO_D_TILT_UP`, or `PELCO_D_TILT_DOWN`.
 * @param[in] tilt_speed  Speed of tilt movement. Must be in the range [0, PELCO_D_MAX_SPEED].
 *
 * @return Returns a `pelco_d_error_t` indicating the success or failure of the operation:
 * - `PELCO_D_SUCCESS` if the message was created successfully.
 * - `PELCO_D_ERROR_NULL_POINTER` if the `msg` pointer is NULL.
 * - `PELCO_D_ERROR_INVALID_ADDRESS` if `address` exceeds `PELCO_D_MAX_ADDRESS`.
 * - `PELCO_D_ERROR_INVALID_SPEED` if `pan_speed` or `tilt_speed` is out of range.
 */
pelco_d_error_t pelco_d_pan_tilt(pelco_d_message_t *msg,
                                 uint8_t address,
                                 pelco_d_pan_direction_t pan_dir,
                                 uint8_t pan_speed,
                                 pelco_d_tilt_direction_t tilt_dir,
                                 uint8_t tilt_speed);

/**
 * @brief Configures a Pelco-D message to control the zoom functionality of a device.
 *
 * This function creates a Pelco-D protocol message for controlling the zoom
 * operation (wide, tele, or stop) based on the specified direction. The message
 * is initialized with appropriate values corresponding to the desired zoom command.
 *
 * @param[out] msg      Pointer to a `pelco_d_message_t` structure where the generated
 *                      zoom message will be stored. Must not be NULL.
 * @param[in] address   Device address for the Pelco-D message. Must be in the range
 *                      [0, PELCO_D_MAX_ADDRESS].
 * @param[in] direction Zoom control direction, which can be one of the following:
 *                      `PELCO_D_ZOOM_WIDE`, `PELCO_D_ZOOM_TELE`, or `PELCO_D_ZOOM_STOP`.
 *
 * @return Returns a `pelco_d_error_t` indicating the success or failure of
 * the operation:
 * - `PELCO_D_SUCCESS` if the message was created successfully.
 * - `PELCO_D_ERROR_NULL_POINTER` if the `msg` pointer is NULL.
 * - `PELCO_D_ERROR_INVALID_ADDRESS` if `address` exceeds `PELCO_D_MAX_ADDRESS`.
 */
pelco_d_error_t pelco_d_zoom(pelco_d_message_t *msg, uint8_t address, pelco_d_zoom_direction_t direction);

/**
 * @brief Constructs a Pelco-D protocol message to query the position (pan or tilt)
 *        of a device with the specified address.
 *
 * This function generates a Pelco-D query position message based on the given
 * parameters. It supports querying either the pan or tilt position depending on
 * the value provided in the `query_pos` parameter.
 *
 * @param[out] msg          Pointer to a `pelco_d_message_t` structure where the
 *                          generated query message will be stored. Must not be NULL.
 * @param[in] address       Device address for the Pelco-D message. Must be in the
 *                          range [0, PELCO_D_MAX_ADDRESS].
 * @param[in] query_pos     Type of position to query: either `PELCO_D_PAN` or
 *                          `PELCO_D_TILT`.
 *
 * @return Returns a `pelco_d_error_t` indicating the success or failure of
 * the operation:
 * - `PELCO_D_SUCCESS` if the message was created successfully.
 * - `PELCO_D_ERROR_NULL_POINTER` if the `msg` pointer is NULL.
 * - `PELCO_D_ERROR_INVALID_ADDRESS` if `address` exceeds `PELCO_D_MAX_ADDRESS`.
 */
pelco_d_error_t pelco_d_query_position(pelco_d_message_t *msg, uint8_t address, pelco_d_query_pos_t query_pos);

/**
 * @brief Prints the contents of a Pelco-D protocol message.
 *
 * This function outputs the formatted representation of a Pelco-D message,
 * including its sync, address, command bytes, data bytes, and checksum. If
 * the input message is NULL, a corresponding warning message is printed.
 *
 * @param[in] msg Pointer to a `pelco_d_message_t` structure containing the message
 *                data to be printed. Must not be NULL.
 */
void pelco_d_print_message(const pelco_d_message_t *msg);

/**
 * @brief Converts an array of bytes to a Pelco-D message.
 *
 * This function parses the given byte array to populate a `pelco_d_message_t` structure.
 * The byte array must have at least `PELCO_D_MESSAGE_SIZE` elements, and the checksum
 * is validated during the conversion.
 *
 * @param[in] buff      Pointer to the input byte array representing the Pelco-D message.
 *                      Must not be NULL.
 * @param[in] size      Size of the input byte array. Must be at least `PELCO_D_MESSAGE_SIZE`.
 * @param[out] msg      Pointer to a `pelco_d_message_t` structure where the parsed
 *                      message will be stored. Must not be NULL.
 *
 * @return Returns a `pelco_d_error_t` indicating the success or failure of
 * the operation:
 * - `PELCO_D_SUCCESS` if the message was converted successfully.
 * - `PELCO_D_ERROR_NULL_POINTER` if either `buff` or `msg` is NULL.
 * - `PELCO_D_ERROR_CHECKSUM` if the size is less than `PELCO_D_MESSAGE_SIZE`.
 */
pelco_d_error_t pelco_d_bytes_to_message(const uint8_t *buff, size_t size, pelco_d_message_t *msg);

/**
 * @brief Converts a Pelco-D message structure to its byte array representation.
 *
 * This function serializes a `pelco_d_message_t` structure into a byte array
 * format suitable for transmission. The function ensures that the provided
 * buffer is not null and has sufficient size to hold the Pelco-D message data.
 *
 * @param[in] msg   Pointer to the `pelco_d_message_t` structure containing
 *                  the message data to be converted. Must not be NULL.
 * @param[out] buff Pointer to a byte array where the serialized message will
 *                  be stored. Must not be NULL.
 * @param[in] size  Size of the destination byte array. Must be greater than or
 *                  equal to `PELCO_D_MESSAGE_SIZE`.
 *
 * @return Returns a `pelco_d_error_t` indicating the success or failure of
 * the operation:
 * - `PELCO_D_SUCCESS` if the message was successfully converted to bytes.
 * - `PELCO_D_ERROR_NULL_POINTER` if `msg` or `buff` is NULL.
 * - `PELCO_D_ERROR_CHECKSUM` if `size` is less than `PELCO_D_MESSAGE_SIZE`.
 */
pelco_d_error_t pelco_d_message_to_bytes(const pelco_d_message_t *msg, uint8_t *buff, size_t size);

/**
 * @brief Prints the contents of a byte buffer in hexadecimal format.
 *
 * This function iterates over the provided byte buffer and prints each byte
 * in hexadecimal format, separated by spaces. If the buffer is NULL, it prints
 * an error message indicating the absence of data.
 *
 * @param[in] buff  Pointer to the buffer containing the bytes to be printed.
 *                  If NULL, a message indicating a null buffer is displayed.
 * @param[in] size  The number of bytes in the buffer to be printed. If this
 *                  is zero, nothing will be printed.
 */
void pelco_d_print_bytes(const uint8_t *buff, size_t size);

#endif