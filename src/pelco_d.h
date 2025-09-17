#ifndef PELCO_D_H
#define PELCO_D_H

#include <stdint.h>
#include <stdio.h>

#define PELCO_D_SYNC_BYTE         0xFF
#define PELCO_D_MESSAGE_SIZE         7
#define PELCO_D_MAX_ADDRESS        255
#define PELCO_D_MAX_SPEED         0x3F
#define PELCO_D_MAX_PRESET         255

#define PELCO_D_CMD1_SENSE        0x80
#define PELCO_D_CMD1_RESERVED1    0x40
#define PELCO_D_CMD1_RESERVED2    0x20
#define PELCO_D_CMD1_AUTO_SCAN    0x10
#define PELCO_D_CMD1_CAMERA_ON_OFF 0x08
#define PELCO_D_CMD1_IRIS_CLOSE   0x04
#define PELCO_D_CMD1_IRIS_OPEN    0x02
#define PELCO_D_CMD1_FOCUS_NEAR   0x01

#define PELCO_D_CMD2_CLEAR_PRESET 0x05
#define PELCO_D_CMD2_CALL_PRESET  0x07
#define PELCO_D_CMD2_SET_AUX      0x09
#define PELCO_D_CMD2_CLEAR_AUX    0x0B
#define PELCO_D_CMD2_DUMMY        0x0D
#define PELCO_D_CMD2_FOCUS_FAR    0x80
#define PELCO_D_CMD2_ZOOM_WIDE    0x40
#define PELCO_D_CMD2_ZOOM_TELE    0x20
#define PELCO_D_CMD2_TILT_DOWN    0x10
#define PELCO_D_CMD2_TILT_UP      0x08
#define PELCO_D_CMD2_PAN_LEFT     0x04
#define PELCO_D_CMD2_PAN_RIGHT    0x02
#define PELCO_D_CMD2_RESERVED     0x01
#define PELCO_D_CMD2_Q_PAN_POS    0x51
#define PELCO_D_CMD2_S_PAN_POS    0x4B
#define PELCO_D_CMD2_Q_TILT_POS   0x53
#define PELCO_D_CMD2_S_TILT_POS   0x4D

#define PELCO_D_EXT_SET_PRESET    0x03
#define PELCO_D_EXT_CLEAR_PRESET  0x05
#define PELCO_D_EXT_GOTO_PRESET   0x07
#define PELCO_D_EXT_FLIP          0x20
#define PELCO_D_EXT_GOTO_ZERO_PAN 0x22
#define PELCO_D_EXT_SET_AUX       0x09
#define PELCO_D_EXT_CLEAR_AUX     0x0B
#define PELCO_D_EXT_REMOTE_RESET  0x0F
#define PELCO_D_EXT_ZONE_START    0x11
#define PELCO_D_EXT_ZONE_END      0x13
#define PELCO_D_EXT_WRITE_CHAR    0x15
#define PELCO_D_EXT_CLEAR_SCREEN  0x17
#define PELCO_D_EXT_ALARM_ACK     0x19

typedef enum {
    PELCO_D_SUCCESS = 0,
    PELCO_D_ERROR_INVALID_ADDRESS,
    PELCO_D_ERROR_INVALID_SPEED,
    PELCO_D_ERROR_INVALID_PRESET,
    PELCO_D_ERROR_INVALID_AUX,
    PELCO_D_ERROR_NULL_POINTER,
    PELCO_D_ERROR_CHECKSUM,
    PELCO_D_ERROR_INVALID_ANGLE,
} pd_err_t;

typedef struct {
    uint8_t sync;
    uint8_t address;
    uint8_t command1;
    uint8_t command2;
    uint8_t data1;
    uint8_t data2;
    uint8_t checksum;
} pd_msg_t;

typedef struct {
    uint8_t sync;
    uint8_t address;
    uint8_t command;
    uint8_t checksum;
} pd_resp_t;

typedef enum {
    PELCO_D_PAN_STOP = 0,
    PELCO_D_PAN_LEFT,
    PELCO_D_PAN_RIGHT,
} pd_pan_dir_t;

typedef enum {
    PELCO_D_TILT_STOP = 0,
    PELCO_D_TILT_UP,
    PELCO_D_TILT_DOWN,
} pd_tilt_dir_t;

typedef enum {
    PELCO_D_ZOOM_STOP = 0,
    PELCO_D_ZOOM_WIDE,
    PELCO_D_ZOOM_TELE,
} pd_zoom_dir_t;

typedef enum {
    PELCO_D_PAN,
    PELCO_D_TILT,
    PELCO_D_ZOOM
} pd_query_pos_t;

/**
 * @brief Creates a Pelco-D protocol message with the specified parameters.
 *
 * This function initializes a Pelco-D message structure by populating it with
 * the given parameters: address, command1, command2, data1, data2, and
 * calculates the checksum for the generated message.
 */
pd_err_t pd_create_message(pd_msg_t *msg, uint8_t addr, uint8_t cmd1, uint8_t cmd2, uint8_t data1, uint8_t data2);

/**
 * @brief Validates a Pelco-D protocol message.
 *
 * This function checks the validity of a given Pelco-D message by verifying
 * its structure, address, synchronization byte, and checksum. The provided
 * message is considered valid if:
 * - It has a valid synchronization byte.
 * - Its address is within the valid range.
 * - Its checksum matches the calculated checksum.
 */
pd_err_t pd_validate_message(const pd_msg_t *msg);

/**
 * @brief Constructs a Pelco-D pan-tilt message with specified parameters.
 *
 * This function generates a Pelco-D message to control pan and tilt operations
 * for a camera. Direction and speed for both pan and tilt movements are
 * configured based on the provided input parameters. The generated message
 * includes the address, command bytes, and speed values.
 */
pd_err_t pd_mv_pan(pd_msg_t *msg, uint8_t addr, pd_pan_dir_t dir, uint8_t spd);

/**
 * @brief Configures a Pelco-D message to control the zoom functionality of a
 * device.
 *
 * This function creates a Pelco-D protocol message for controlling the zoom
 * operation (wide, tele, or stop) based on the specified direction. The message
 * is initialized with appropriate values corresponding to the desired zoom
 * command.
 */
pd_err_t pd_zoom(pd_msg_t *msg, uint8_t addr, pd_zoom_dir_t dir);

/**
 * @brief Constructs a Pelco-D protocol message to query the position (pan or
 * tilt) of a device with the specified address.
 *
 * This function generates a Pelco-D query position message based on the given
 * parameters. It supports querying either the pan or tilt position depending on
 * the value provided in the `query_pos` parameter.
 */
pd_err_t pd_query_pos(pd_msg_t *msg, uint8_t addr, pd_query_pos_t query_pos);

/**
 * @brief Prints the contents of a Pelco-D protocol message.
 *
 * This function outputs the formatted representation of a Pelco-D message,
 * including its sync, address, command bytes, data bytes, and checksum. If
 * the input message is NULL, a corresponding warning message is printed.
 */
void pd_print_msg(const pd_msg_t *msg);

/**
 * @brief Converts an array of bytes to a Pelco-D message.
 *
 * This function parses the given byte array to populate a `pd_msg_t`
 * structure. The byte array must have at least `PELCO_D_MESSAGE_SIZE` elements,
 * and the checksum is validated during the conversion.
 */
pd_err_t pd_pack_bytes(const uint8_t *buff, size_t size, pd_msg_t *msg);

/**
 * @brief Converts a Pelco-D message structure to its byte array representation.
 *
 * This function serializes a `pd_msg_t` structure into a byte array
 * format suitable for transmission. The function ensures that the provided
 * buffer is not null and has sufficient size to hold the Pelco-D message data.
 */
pd_err_t pd_pack_message(const pd_msg_t *msg, uint8_t *buff, size_t size);

/**
 * @brief Prints the contents of a byte buffer in hexadecimal format.
 *
 * This function iterates over the provided byte buffer and prints each byte
 * in hexadecimal format, separated by spaces. If the buffer is NULL, it prints
 * an error message indicating the absence of data.
 */
void pd_print_bytes(const uint8_t *buff, size_t size);

pd_err_t pd_set_pan(pd_msg_t *msg, uint8_t addr, int angle);

pd_err_t pd_get_pan_angle(pd_msg_t *msg, int *pan);

pd_err_t pd_get_tilt_angle(pd_msg_t *msg, int *tilt);

/**
 * @brief Calculates the checksum for a given Pelco-D message.
 *
 * The checksum is computed as the sum of the message's address, command1, command2,
 * data1, and data2 fields, modulo 256.
 */
uint8_t pd_checksum(const pd_msg_t *msg);

#endif
