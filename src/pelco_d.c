#include "pelco_d.h"

/**
 * @brief Calculates the checksum for a given Pelco-D message.
 *
 * The checksum is computed as the sum of the message's address, command1, command2,
 * data1, and data2 fields, modulo 256.
 */
uint8_t pd_checksum(const pd_msg_t *msg) {
    if (!msg) {
        return 0;
    }
    return (msg->address + msg->command1 + msg->command2 + msg->data1 + msg->data2) % 256;
}

void into_bytes(uint16_t val, uint8_t *b1, uint8_t *b2) {
    *b1 = (val >> 8) & 0xFF;
    *b2 = (val     ) & 0xFF;
}

pd_err_t pd_get_pan_angle(pd_msg_t *msg, int *pan) {
    if (!msg) {
        return PELCO_D_ERROR_NULL_POINTER;
    }
    if (msg->command2 != 0x59) {
        return PELCO_D_ERROR_CHECKSUM;
    }
    const int angle = (msg->data1 * 256 + msg->data2) / 100;
    *pan = angle;
    return PELCO_D_SUCCESS;
}

pd_err_t pd_get_tilt_angle(pd_msg_t *msg, int *tilt) {
    int angle = 0;

    if (!msg) {
        return PELCO_D_ERROR_NULL_POINTER;
    }
    if (msg->command2 != 0x5B) {
        return PELCO_D_ERROR_CHECKSUM;
    }
    const int tilt_data = msg->data1 * 256 + msg->data2;
    if (tilt_data > 18000) {
        angle = (36000 - tilt_data) / 100;
    } else if (tilt_data < 18000) {
        angle = -(tilt_data / 100);
    }
    *tilt = angle;
    return PELCO_D_SUCCESS;
}

pd_err_t pd_create_message(pd_msg_t *msg, uint8_t addr, uint8_t cmd1, uint8_t cmd2, uint8_t data1, uint8_t data2) {
    if (!msg) {
        return PELCO_D_ERROR_NULL_POINTER;
    }
    if (addr > PELCO_D_MAX_ADDRESS) {
        return PELCO_D_ERROR_INVALID_ADDRESS;
    }

    msg->sync = PELCO_D_SYNC_BYTE;
    msg->address = addr;
    msg->command1 = cmd1;
    msg->command2 = cmd2;
    msg->data1 = data1;
    msg->data2 = data2;
    msg->checksum = pd_checksum(msg);

    return PELCO_D_SUCCESS;
}

pd_err_t pd_validate_message(const pd_msg_t *msg) {
    if (!msg) {
        return PELCO_D_ERROR_NULL_POINTER;
    }
    if (msg->sync != PELCO_D_SYNC_BYTE) {
        return PELCO_D_ERROR_CHECKSUM;
    }
    if (msg->address > PELCO_D_MAX_ADDRESS) {
        return PELCO_D_ERROR_INVALID_ADDRESS;
    }
    uint8_t checksum = pd_checksum(msg);
    if (checksum != msg->checksum) {
        return PELCO_D_ERROR_CHECKSUM;
    }
    return PELCO_D_SUCCESS;
}

pd_err_t pd_mv_pan(pd_msg_t *msg, uint8_t addr, pd_pan_dir_t dir, uint8_t spd) {
    if (!msg) {
        return PELCO_D_ERROR_NULL_POINTER;
    }
    if (addr > PELCO_D_MAX_ADDRESS) {
        return PELCO_D_ERROR_INVALID_ADDRESS;
    }
    if (spd > PELCO_D_MAX_SPEED) {
        return PELCO_D_ERROR_INVALID_SPEED;
    }

    uint8_t cmd1 = 0x00;
    uint8_t cmd2 = 0x00;

    switch (dir) {
        case PELCO_D_PAN_LEFT:
            cmd2 = PELCO_D_CMD2_PAN_LEFT;
            break;
        case PELCO_D_PAN_RIGHT:
            cmd2 = PELCO_D_CMD2_PAN_RIGHT;
            break;
        case PELCO_D_PAN_STOP:
        default:
            break;
    }

    return pd_create_message(msg, addr, cmd1, cmd2, 0x00, spd);
}

pd_err_t pd_mv_tilt(pd_msg_t *msg, uint8_t addr, pd_tilt_dir_t dir, uint8_t spd) {
    if (!msg) {
        return PELCO_D_ERROR_NULL_POINTER;
    }
    if (addr > PELCO_D_MAX_ADDRESS) {
        return PELCO_D_ERROR_INVALID_ADDRESS;
    }
    if (spd > PELCO_D_MAX_SPEED) {
        return PELCO_D_ERROR_INVALID_SPEED;
    }
    uint8_t cmd1 = 0x00;
    uint8_t cmd2 = 0x00;

    switch (dir) {
        case PELCO_D_TILT_UP:
            cmd2 = PELCO_D_CMD2_TILT_UP;
            break;
        case PELCO_D_TILT_DOWN:
            cmd2 = PELCO_D_CMD2_TILT_DOWN;
            break;
        case PELCO_D_TILT_STOP:
            break;
    }

    return pd_create_message(msg, addr, cmd1, cmd2, 0x00, spd);
}

pd_err_t pd_zoom(pd_msg_t *msg, uint8_t addr, pd_zoom_dir_t dir) {
    if (!msg) {
        return PELCO_D_ERROR_NULL_POINTER;
    }
    uint8_t cmd1 = 0x00;
    uint8_t cmd2 = 0x00;
    switch (dir) {
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

    return pd_create_message(msg, addr, cmd1, cmd2, 0x00, 0x00);
}

pd_err_t pd_query_pos(pd_msg_t *msg, uint8_t addr, pd_query_pos_t query_pos) {
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
    return pd_create_message(msg, addr, cmd1, cmd2, 0x00, 0x00);
}

pd_err_t pd_set_pan(pd_msg_t *msg, uint8_t addr, int angle) {
    if (!msg) {
        return PELCO_D_ERROR_NULL_POINTER;
    }
    if (addr > PELCO_D_MAX_ADDRESS) {
        return PELCO_D_ERROR_INVALID_ADDRESS;
    }
    uint8_t data1, data2;
    into_bytes(angle, &data1, &data2);

    return pd_create_message(msg, addr, 0x00, 0x4B, data1, data2);
}

pd_err_t pd_set_tilt(pd_msg_t *msg, uint8_t addr, int angle) {
    if (!msg) {
        return PELCO_D_ERROR_NULL_POINTER;
    }
    if (addr > PELCO_D_MAX_ADDRESS) {
        return PELCO_D_ERROR_INVALID_ADDRESS;
    }
    uint8_t data1, data2;
    into_bytes(angle, &data1, &data2);

    return pd_create_message(msg, addr, 0x00, 0x4B, data1, data2);
}

void pd_print_msg(const pd_msg_t *msg) {
    if (!msg) {
        printf("NULL pelco_d message.\n");
        return;
    }
    printf("Pelco-D Message: %02X %02X %02X %02X %02X %02X %02X\n",
        msg->sync, msg->address, msg->command1, msg->command2, msg->data1, msg->data2, msg->checksum);
}

pd_err_t pd_pack_bytes(const uint8_t *buff, size_t size, pd_msg_t *msg) {
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

    return pd_checksum(msg);
}

pd_err_t pd_pack_message(const pd_msg_t *msg, uint8_t *buff, size_t size) {
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

void pd_print_bytes(const uint8_t *buff, size_t size) {
    if (!buff) {
        printf("NULL pelco_d bytes.\n");
        return;
    }
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", buff[i]);
    }
    printf("\n");
}