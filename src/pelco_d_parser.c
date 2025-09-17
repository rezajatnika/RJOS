#include "pelco_d_parser.h"

#include <string.h>

pd_parser_err_t pd_parse_stream(pd_parser_t *parser, const uint8_t *data, size_t size, pd_parsed_msg_t *pmsg) {
    if (!parser || !data || !pmsg) {
        return PD_PARSER_ERROR_NULL_POINTER;
    }

    for (size_t i = 0; i < size; i++) {
        uint8_t byte = data[i];

        switch (parser->state) {
            case PD_PARSER_STATE_SYNC:
                if (byte == PELCO_D_SYNC_BYTE) {
                    parser->message.sync = byte;
                    parser->buffer_pos = 0;
                    parser->state = PD_PARSER_STATE_ADDRESS;
                }
                break;

            case PD_PARSER_STATE_ADDRESS:
                parser->message.address = byte;
                parser->state = PD_PARSER_STATE_COMMAND1;
                break;

            case PD_PARSER_STATE_COMMAND1:
                parser->message.command1 = byte;
                parser->state = PD_PARSER_STATE_COMMAND2;
                break;

            case PD_PARSER_STATE_COMMAND2:
                parser->message.command2 = byte;
                parser->state = PD_PARSER_STATE_DATA1;
                break;

            case PD_PARSER_STATE_DATA1:
                parser->message.data1 = byte;
                parser->state = PD_PARSER_STATE_DATA2;
                break;

            case PD_PARSER_STATE_DATA2:
                parser->message.data2 = byte;
                parser->state = PD_PARSER_STATE_CHECKSUM;
                break;

            case PD_PARSER_STATE_CHECKSUM:
                parser->message.checksum = byte;
                parser->state = PD_PARSER_STATE_COMPLETE;

                if (pd_validate_message(&parser->message) == PELCO_D_SUCCESS) {
                    pd_parser_err_t result = pd_classify_message(&parser->message, pmsg);
                    parser->messages_parsed++;
                    parser->state = PD_PARSER_STATE_SYNC;

                    if (result == PD_PARSER_SUCCESS) {
                        return PD_PARSER_SUCCESS;
                    }
                } else {
                    parser->parser_errors++;
                }
                parser->state = PD_PARSER_STATE_SYNC;
                break;

            case PD_PARSER_STATE_COMPLETE:
                parser->state = PD_PARSER_STATE_SYNC;
                break;
        }
    }
    return PD_PARSER_ERROR_INCOMPLETE_MESSAGE;
}

pd_parser_err_t pd_classify_message(const pd_msg_t *msg, pd_parsed_msg_t *parsed_msg) {
    if (!msg || !parsed_msg) {
        return PD_PARSER_ERROR_NULL_POINTER;
    }

    memset(parsed_msg, 0, sizeof(pd_parsed_msg_t));
    parsed_msg->address = msg->address;
    parsed_msg->raw_message = *msg;
    parsed_msg->type = 0; /* Unknown message. */

    uint8_t cmd1 = msg->command1;
    uint8_t cmd2 = msg->command2;

    if (cmd2 == 0x59) {
        parsed_msg->type = PD_MSG_TYPE_RESPONSE_PAN;
        int angle;
        if (pd_get_pan_angle((pd_msg_t*)msg, &angle) == PELCO_D_SUCCESS) {
            parsed_msg->position.angle_pan = angle;
            parsed_msg->position.valid = 1;
        }
    }

    if (cmd2 == 0x5B) {
        parsed_msg->type = PD_MSG_TYPE_RESPONSE_TILT;
        int angle;
        if (pd_get_tilt_angle((pd_msg_t*)msg, &angle) == PELCO_D_SUCCESS) {
            parsed_msg->position.angle_tilt = angle;
            parsed_msg->position.valid = 1;
        }
    }

    return PD_PARSER_SUCCESS;
}
