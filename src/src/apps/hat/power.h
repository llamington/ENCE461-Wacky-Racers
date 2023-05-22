#ifndef POWER_H
#define POWER_H

#include "pio.h"

void power_init(void);

bool is_low_battery(void);

void low_power_mode(void);

#endif