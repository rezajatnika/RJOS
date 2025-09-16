#include "pelco_d.h"

uint8_t pelco_d_checksum(const pelco_d_message_t *msg) {
    if (!msg) {
        return 0;
    }
    return (msg->address + msg->command1 + msg->command2 + msg->data1 + msg->data2) % 256;
}

pelco_d_error_t pelco_d_create_message(pelco_d_message_t *msg,
    uint8_t address,
    uint8_t command1,
    uint8_t command2,
    uint8_t data1,
    uint8_t data2) {
    if (!msg) {
        return PELCO_D_ERROR_NULL_POINTER;
    }
    if (address > PELCO_D_MAX_ADDRESS) {
        return PELCO_D_ERROR_INVALID_ADDRESS;
    }

    msg->sync = PELCO_D_SYNC_BYTE;
    msg->address = address;
    msg->command1 = command1;
    msg->command2 = command2;
    msg->data1 = data1;
    msg->data2 = data2;
    msg->checksum = pelco_d_checksum(msg);

    return PELCO_D_SUCCESS;
}

pelco_d_error_t pelco_d_validate_message(const pelco_d_message_t *msg) {
    if (!msg) {
        return PELCO_D_ERROR_NULL_POINTER;
    }
    if (msg->sync != PELCO_D_SYNC_BYTE) {
        return PELCO_D_ERROR_CHECKSUM;
    }
    if (msg->address > PELCO_D_MAX_ADDRESS) {
        return PELCO_D_ERROR_INVALID_ADDRESS;
    }
    uint8_t checksum = pelco_d_checksum(msg);
    if (checksum != msg->checksum) {
        return PELCO_D_ERROR_CHECKSUM;
    }
    return PELCO_D_SUCCESS;
}

pelco_d_error_t pelco_d_pan_tilt(pelco_d_message_t *msg,
    uint8_t address,
    pelco_d_pan_direction_t pan_dir,
    uint8_t pan_speed,
    pelco_d_tilt_direction_t tilt_dir,
    uint8_t tilt_speed) {
    if (!msg) {
        return PELCO_D_ERROR_NULL_POINTER;
    }
    if (address > PELCO_D_MAX_ADDRESS) {
        return PELCO_D_ERROR_INVALID_ADDRESS;
    }
    if (pan_speed > PELCO_D_MAX_SPEED || tilt_speed > PELCO_D_MAX_SPEED) {
        return PELCO_D_ERROR_INVALID_SPEED;
    }

    uint8_t cmd1 = 0x00;
    uint8_t cmd2 = 0x00;

    switch (pan_dir) {
        case PELCO_D_PAN_LEFT:
            cmd2 |= PELCO_D_CMD2_PAN_LEFT;
            break;
        case PELCO_D_PAN_RIGHT:
            cmd2 |= PELCO_D_CMD2_PAN_RIGHT;
            break;
        case PELCO_D_PAN_STOP:
        default:
            break;
    }

    switch (tilt_dir) {
        case PELCO_D_TILT_UP:
            cmd2 |= PELCO_D_CMD2_TILT_UP;
            break;
        case PELCO_D_TILT_DOWN:
            cmd2 |= PELCO_D_CMD2_TILT_DOWN;
            break;
        case PELCO_D_TILT_STOP:
        default:
            break;
    }

    return pelco_d_create_message(msg, address, cmd1, cmd2, pan_speed, tilt_speed);
}

pelco_d_error_t pelco_d_zoom(pelco_d_message_t *msg, uint8_t address, pelco_d_zoom_direction_t direction) {
    if (!msg) {
        return PELCO_D_ERROR_NULL_POINTER;
    }
    uint8_t cmd1 = 0x00;
    uint8_t cmd2 = 0x00;
    switch (direction) {
        case PELCO_D_ZOOM_WIDE:
            cmd2 |= PELCO_D_CMD2_ZOOM_WIDE;
            break;
        case PELCO_D_ZOOM_TELE:
            cmd2 |= PELCO_D_CMD2_ZOOM_TELE;
            break;
        case PELCO_D_ZOOM_STOP:
        default:
            break;
    }

    return pelco_d_create_message(msg, address, cmd1, cmd2, 0x00, 0x00);
}

pelco_d_error_t pelco_d_query_position(pelco_d_message_t *msg, uint8_t address, pelco_d_query_pos_t query_pos) {
    if (!msg) {
        return PELCO_D_ERROR_NULL_POINTER;
    }
    uint8_t cmd1 = 0x00;
    uint8_t cmd2 = 0x00;
    switch (query_pos) {
        case PELCO_D_PAN:
            cmd2 |= PELCO_D_CMD2_Q_PAN_POS;
            break;
        case PELCO_D_TILT:
            cmd2 |= PELCO_D_CMD2_Q_TILT_POS;
            break;
        default:
            break;
    }
    return pelco_d_create_message(msg, address, cmd1, cmd2, 0x00, 0x00);
}

void pelco_d_print_message(const pelco_d_message_t *msg) {
    if (!msg) {
        printf("NULL pelco_d message.\n");
        return;
    }
    printf("Pelco-D Message: %02X %02X %02X %02X %02X %02X %02X\n",
        msg->sync, msg->address, msg->command1, msg->command2, msg->data1, msg->data2, msg->checksum);
}

pelco_d_error_t pelco_d_bytes_to_message(const uint8_t *buff, size_t size, pelco_d_message_t *msg) {
    if (!buff || !msg) {
        return PELCO_D_ERROR_NULL_POINTER;
    }
    if (size < PELCO_D_MESSAGE_SIZE) {
        return PELCO_D_ERROR_CHECKSUM;
    }
    msg->sync = buff[0];
    msg->address = buff[1];
    msg->command1 = buff[2];
    msg->command2 = buff[3];
    msg->data1 = buff[4];
    msg->data2 = buff[5];
    msg->checksum = buff[6];

    return pelco_d_checksum(msg);
}

pelco_d_error_t pelco_d_message_to_bytes(const pelco_d_message_t *msg, uint8_t *buff, size_t size) {
    if (!msg || !buff) {
        return PELCO_D_ERROR_NULL_POINTER;
    }
    if (size < PELCO_D_MESSAGE_SIZE) {
        return PELCO_D_ERROR_CHECKSUM;
    }
    buff[0] = msg->sync;
    buff[1] = msg->address;
    buff[2] = msg->command1;
    buff[3] = msg->command2;
    buff[4] = msg->data1;
    buff[5] = msg->data2;
    buff[6] = msg->checksum;
    return PELCO_D_SUCCESS;
}

void pelco_d_print_bytes(const uint8_t *buff, size_t size) {
    if (!buff) {
        printf("NULL pelco_d bytes.\n");
        return;
    }
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", buff[i]);
    }
    printf("\n");
}