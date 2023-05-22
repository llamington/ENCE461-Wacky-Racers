#ifndef LEDS_H
#define LEDS_H

void leds_init(void);

void led_toggle_status(void);

void led_tape_update(void);

void led_toggle_error(void);

void led_off_status(void);

void leds_shutdown(void);

#endif