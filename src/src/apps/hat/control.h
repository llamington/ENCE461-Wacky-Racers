#ifndef CONTROL_H
#define CONTROL_H

#include <stdint.h>

/** Control methods */
typedef enum
{
    CONTROL_IMU,
    CONTROL_JOYSTICK
} control_t;

void control_init(void);

void poll_control_method(void);

void poll_direction(void);

void read_direction(int8_t *throttle, int8_t *heading);

void control_shutdown(void);

#endif