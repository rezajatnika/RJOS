#ifndef DATPRO_H
#define DATPRO_H

typedef struct {
    int camera_pan;
    int camera_tilt;
    int camera_zoom;
    int turret_azimuth;
    int turret_elevation;
    int imu_roll;
    int imu_pitch;
    int imu_yaw;
} datpro_t;

#endif