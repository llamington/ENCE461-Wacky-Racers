#ifndef BUTTONS_H
#define BUTTONS_h

#include "pio.h"

void buttons_init(void);

void buttons_poll(void);

bool sleep_btn_pushed(void);

bool sleep_btn_held(void);

bool joy_btn_pushed(void);

bool joy_btn_held(void);

#endif