#include "pio.h"
#include "joystick.h"
#include "adc.h"
#include "panic.h"
#include "common.h"
#include "ring.h"

#define RING_BUF_SIZE 11

/** Joystick ADC parameter values */
#define X_MAX 3925
#define X_MIN 290
#define Y_MAX 3925
#define Y_MIN 90

static const adc_cfg_t adc_cfg =
    {
        .bits = 12,
        .channels = BIT(JOYSTICK_X_ADC_CHANNEL) | BIT(JOYSTICK_Y_ADC_CHANNEL),
        .trigger = ADC_TRIGGER_SW,
        .clock_speed_kHz = 1000};

static adc_t adc;

/** Ring buffers for joystick x, y values */
static ring_t *ring_buf[2];

/** Initialise Joystick */
void joystick_init(void)
{

    pio_config_set(JOYSTICK_BUTTON_PIO, PIO_PULLUP);
    adc = adc_init(&adc_cfg);
    if (!adc)
        panic(LED_ERROR_PIO, ERROR_CODE_JOY_ADC);

    /** Initialise ring buffers */
    for (int i = 0; i < 2; i++)
        ring_buf[i] = ring_init(NULL, NULL, RING_BUF_SIZE * sizeof(uint16_t));
}

/** Polls for the current direction from the joystick */
void joystick_poll_direction(void)
{
    uint16_t adc_data[2];
    adc_read(adc, adc_data, sizeof(adc_data));

    ring_write_continuous(ring_buf[0], &adc_data[0], sizeof(uint16_t));
    ring_write_continuous(ring_buf[1], &adc_data[1], sizeof(uint16_t));
}

/** Reads the current direction from the joystick */
void joystick_read_direction(int8_t *throttle, int8_t *heading)
{
    ring_size_t num_samples = ring_read_num(ring_buf[0]) / sizeof(uint16_t);

    uint16_t x_avg = 0;
    uint16_t y_avg = 0;
    uint16_t temp = 0;

    for (uint8_t i = 0; i < num_samples; i++)
    {
        ring_read(ring_buf[0], &temp, sizeof(uint16_t));
        x_avg += temp;
        ring_read(ring_buf[1], &temp, sizeof(uint16_t));
        y_avg += temp;
    }

    x_avg /= num_samples;
    y_avg /= num_samples;

    /** Constrain to ranges */
    x_avg = MIN(X_MAX, MAX(X_MIN, x_avg));
    y_avg = MIN(Y_MAX, MAX(Y_MIN, y_avg));

    *throttle = map_range(X_MIN, X_MAX, 127.0, -127.0, x_avg);
    *heading = map_range(Y_MIN, Y_MAX, -127.0, 127.0, y_avg);

#ifdef SERIAL_DATA
    // printf("throttle: %d\n", *throttle);
    // printf("heading: %d\n", *heading);
    // printf("x_avg: %d\n", x_avg);
    // printf("y_avg: %d\n", y_avg);
#endif
}

void joystick_shutdown(void)
{
    adc_shutdown(adc);
}