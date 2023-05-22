#include "pio.h"
#include "led.h"
#include "ledbuffer.h"

#define TAPE_LEDS 26 // Number of LEDs on LED Tape

/* Define status LED configuration.  */
static const led_cfg_t led_status_cfg =
    {
        .pio = LED_STATUS_PIO,
        .active = 1};

/* Define error LED configuration.  */
static const led_cfg_t led_error_cfg =
    {
        .pio = LED_ERROR_PIO,
        .active = 1};

static led_t led_status;
static led_t led_error;

static ledbuffer_t *led_buffer;
static uint8_t led_addr = 0;

void leds_init(void)
{
    led_status = led_init(&led_status_cfg);
    led_error = led_init(&led_error_cfg);
    led_set(led_status, 1);
    led_buffer = ledbuffer_init(LEDTAPE_PIO, TAPE_LEDS);
}

void led_toggle_status(void)
{
    led_toggle(led_status);
}

void led_toggle_error(void)
{
    led_toggle(led_error);
}

void led_off_status(void)
{
    led_set(led_status, 0);
}

void led_tape_update(void)
{
    if (led_addr++ >= TAPE_LEDS)
    {
        ledbuffer_clear(led_buffer);
        led_addr = 0;
    }
    ledbuffer_set(led_buffer, 0, 0, 0, 255);
    ledbuffer_write(led_buffer);
    ledbuffer_advance(led_buffer, 1);
}

void leds_shutdown(void)
{
    led_off_status();
    ledbuffer_clear(led_buffer);
    led_tape_update();
}