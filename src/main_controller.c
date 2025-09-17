#include "macros.h"
#include "pelco_d.h"
#include "pelco_d_parser.h"
#include "ptu.h"
#include "rjos.h"
#include "scheduler.h"
#include "serial.h"
#include "udp.h"

#define NODEBUG_MODE 0

/* PTU Device Type */
ptu_t camera;
ptu_t turret;

/* Serial Devices */
serial_t com_camera;
serial_t com_turret;

/* PTU Parser */
pd_parser_t camera_parser;
pd_parser_t turret_parser;

/* UDP Sender Type */
udp_t udp_sender;

void serial_setup() {
#if NODEBUG_MODE
    serial_open(&com_camera, "/dev/ttyUSB01", 9600);
    serial_open(&com_turret, "/dev/ttyUSB02", 19200);
#endif
}

void req_turret_azm_pos(void *args) {
    pd_msg_t msg;
    pd_query_pos(&msg, PTU_TURRET_ADDR, PELCO_D_PAN);

    uint8_t buff[7];
    pd_pack_message(&msg, buff, 7);
#if NODEBUG_MODE
    serial_write(&com_turret, buff, 7);
#else
    pd_print_bytes(buff, 7);
#endif
}

void req_turret_ele_pos(void *args) {
    pd_msg_t msg;
    pd_query_pos(&msg, PTU_TURRET_ADDR, PELCO_D_TILT);

    uint8_t buff[7];
    pd_pack_message(&msg, buff, 7);
#if NODEBUG_MODE
    serial_write(&com_turret, buff, 7);
#else
    pd_print_bytes(buff, 7);
#endif
}

void monitor_turret_pos(void *args) {
    uint8_t recv[256];
    size_t bytes_available = 0;

#if NODEBUG_MODE
    serial_bytes_available(&com_turret, &bytes_available);
    if (bytes_available > 0) {
        serial_read(&com_turret, recv, bytes_available);

        pd_parsed_msg_t parsed;
        pd_parser_err_t result = pd_parse_stream(&turret_parser, recv, bytes_available, &parsed);

        if (result == PD_PARSER_SUCCESS) {
            pd_print_msg(&parsed.raw_message);
        } else {
            printf("Error parsing message: %d\n", result);
        }
    }
#else
    printf("Debug mode: no serial communication.\n");
#endif
}

void set_turret_movement(void *args) {
    /* Set turret movement. */
}

void monitor_imu(void *args) {
    /* Monitor IMU function. */
}

/**********************************************************************************************************************/
/**********************************************************************************************************************/

int main(void) {
    /* System initialization. */
    rjos_init("config.txt", "log.txt");
    /* Serial initialization. */
    serial_setup();
    /* Scheduler initialization */
    sched_init(16);

    /* Tasks list: */
    SCHED_ADD_TASK(req_turret_azm_pos,  NULL, 500, 0, "req_turret_azm_pos");
    SCHED_ADD_TASK(req_turret_ele_pos,  NULL, 500, 0, "req_turret_ele_pos");
    SCHED_ADD_TASK(monitor_turret_pos,  NULL, 100, 0, "monitor_turret_pos");
    SCHED_ADD_TASK(set_turret_movement, NULL, 500, 0, "set_turret_movement");
    SCHED_ADD_TASK(monitor_imu,         NULL, 100, 0, "monitor_imu");

    /* Setup scheduler log callback and signal handlers. */
    sched_set_log_hook(NULL);
    sched_setup_signal_handlers();
    sched_start();

    /* Clean up scheduler. */
    sched_destroy();

    /* Close serial ports. */
    serial_close(&com_camera);
    serial_close(&com_turret);

    /* System cleanup. */
    rjos_cleanup();
}

/**********************************************************************************************************************/
/**********************************************************************************************************************/
