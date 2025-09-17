#ifndef PELCO_D_PARSER_H
#define PELCO_D_PARSER_H

#include "pelco_d.h"

typedef enum {
    PD_PARSER_SUCCESS                  =  0,
    PD_PARSER_ERROR_NULL_POINTER       = -1,
    PD_PARSER_ERROR_INVALID_SIZE       = -2,
    PD_PARSER_ERROR_SYNC_NOT_FOUND     = -3,
    PD_PARSER_ERROR_INCOMPLETE_MESSAGE = -4,
    PD_PARSER_ERROR_INVALID_CHECKSUM   = -5,
    PD_PARSER_ERROR_BUFFER_FULL        = -6
} pd_parser_err_t;

typedef enum {
    PD_PARSER_STATE_SYNC = 0,
    PD_PARSER_STATE_ADDRESS,
    PD_PARSER_STATE_COMMAND1,
    PD_PARSER_STATE_COMMAND2,
    PD_PARSER_STATE_DATA1,
    PD_PARSER_STATE_DATA2,
    PD_PARSER_STATE_CHECKSUM,
    PD_PARSER_STATE_COMPLETE
} pd_parser_state_t;

typedef struct {
    pd_msg_t message;
    pd_parser_state_t state;
    uint8_t buffer[256];
    size_t buffer_pos;
    size_t bytes_expected;
    uint32_t messages_parsed;
    uint32_t parser_errors;
} pd_parser_t;

typedef enum {
    PD_MSG_TYPE_RESPONSE = 0,
    PD_MSG_TYPE_MOVEMENT,
    PD_MSG_TYPE_ZOOM,           /* Zoom command */
    PD_MSG_TYPE_QUERY_PAN,      /* Query pan position */
    PD_MSG_TYPE_QUERY_TILT,     /* Query tilt position */
    PD_MSG_TYPE_RESPONSE_PAN,   /* Pan position response */
    PD_MSG_TYPE_RESPONSE_TILT,  /* Tilt position response */
    PD_MSG_TYPE_PRESET,         /* Preset command */
    PD_MSG_TYPE_AUX             /* Auxiliary command */
} pd_msg_type_t;

typedef struct {
    pd_msg_type_t type;
    uint8_t address;

    struct {
        pd_pan_dir_t pan;
        pd_tilt_dir_t tilt;
        uint8_t pan_speed;
        uint8_t tilt_speed;
    } movement;

    struct {
        pd_zoom_dir_t dir;
    } zoom;

    struct {
        int angle_pan;
        int angle_tilt;
        int valid;
    } position;

    pd_msg_t raw_message;
} pd_parsed_msg_t;

pd_parser_err_t pd_parse_stream(pd_parser_t *parser, const uint8_t *buff, size_t size, pd_parsed_msg_t *pmsg);

/**
 * @brief Parse a single message and classify its type
 *
 * @param msg Pointer to Pelco-D message structure
 * @param parsed_msg Pointer to store parsed message information
 * @return Error code
 */
pd_parser_err_t pd_classify_message(const pd_msg_t *msg, pd_parsed_msg_t *parsed_msg);


#endif