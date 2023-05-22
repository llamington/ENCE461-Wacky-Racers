#ifndef RADIO_H
#define RADIO_H

#include <stdint.h>
#include <pio.h>

void radio_init(void);

void radio_transmit_direction(int8_t throttle, int8_t heading);

bool radio_read_bumper(void);

uint8_t radio_get_channel(void);

void radio_poll_channel(void);

void radio_shutdown(void);

void radio_transmit_dastardly(void);

#endif