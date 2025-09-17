#include "pelco_d.h"
#include "ptu.h"

static pd_msg_t camera_msg = { 0, 0, 0, 0, 0, 0, 0 };
static pd_msg_t turret_msg = { 0, 0, 0, 0, 0, 0, 0 };

int get_camera_pan_pos() {
    return 0;
}