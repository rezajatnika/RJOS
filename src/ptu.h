#ifndef PTU_H
#define PTU_H

#define PTU_CAMERA_ADDR 0x01
#define PTU_CAMERA_BDRT 9600

#define PTU_TURRET_ADDR 0x02
#define PTU_TURRET_BDRT 19200

typedef struct {
    int pan;
    int tilt;
    int zoom;
} ptu_t;

#endif