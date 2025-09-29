#ifndef _RCWS_HANDLER_H_
#define _RCWS_HANDLER_H_

#ifdef __cplusplus
extern "C" {
#endif

    int handler_barrel_set_azimuth(float setpoint);
    int handler_barrel_set_elevation(float setpoint);
    int handler_barrel_manual_ctrl_azimuth(int direction);
    int handler_barrel_manual_ctrl_elevation(int direction);

#ifdef __cplusplus
}
#endif
#endif
