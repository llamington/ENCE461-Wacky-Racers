#include "power.h"
#include "adc.h"
#include "panic.h"
#include "pwm.h"
#include "leds.h"
#include "common.h"
#include "control.h"
#include "delay.h"
#include "radio.h"

#define LOW_BAT_THRESHOLD 2650
// #define LOW_POWER_MODE // define to enable low power mode
#define LED_FLASH_DELAY 1000

static const adc_cfg_t bat_adc_cfg =
    {
        .bits = 12,
        .channels = BIT(BATTERY_ADC_CHANNEL),
        .trigger = ADC_TRIGGER_SW,
        .clock_speed_kHz = 1000};

static adc_t bat_adc;

/** Initialise power */
void power_init(void)
{
    bat_adc = adc_init(&bat_adc_cfg);
    if (!bat_adc)
        panic(LED_ERROR_PIO, ERROR_CODE_BAT_ADC);
}

/** Poll ADC to determine if battery is low */
bool is_low_battery(void)
{
    uint16_t adc_data[1];
    adc_read(bat_adc, adc_data, sizeof(*adc_data));
    return (*adc_data < LOW_BAT_THRESHOLD) ? true : false;
}

/** Enter a low-power mode for reduced power draw */
void low_power_mode(void)
{
#ifndef LOW_POWER_MODE
    return;
#else
    leds_shutdown();
    adc_shutdown(bat_adc);
    radio_shutdown();
    pwm_shutdown();
    control_shutdown();

    while (1)
    {
        delay_ms(LED_FLASH_DELAY);
        led_toggle_error();
    }
#endif
}