#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdio.h>

void joystick_init(void);

void joystick_poll_direction(void);

void joystick_read_direction(int8_t *throttle, int8_t *heading);

void joystick_shutdown(void);

#endif