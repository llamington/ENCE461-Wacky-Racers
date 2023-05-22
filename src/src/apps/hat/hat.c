
#include "target.h"
#include "pacer.h"
#include "radio.h"
#include "control.h"
#include "leds.h"
#include "usb_serial.h"
#include "buttons.h"
#include "common.h"
#include "buzzer.h"
#include "power.h"
#include "imu.h"

int main(void)
{
    uint16_t loop_count = 1;
    int8_t throttle, heading;

    pacer_init(PACER_RATE);
    buttons_init();
    radio_init();
    leds_init();
    buzzer_init();
    power_init();

#ifdef SERIAL_DATA
    usb_serial_stdio_init();
#endif

    control_init();

    while (1)
    {
        pacer_wait();
        if (should_run(BUZZER_UPDATE_RATE, loop_count))
            buzzer_update();
        if (should_run(CH_POLL_RATE, loop_count))
            radio_poll_channel();
        if (should_run(CONTROL_METHOD_RATE, loop_count))
            poll_control_method();
        if (should_run(DIRECTION_POLL_RATE, loop_count))
            poll_direction();
        if (should_run(DIRECTION_READ_RATE, loop_count))
            read_direction(&throttle, &heading);
        if (should_run(LED_FLASH_RATE, loop_count))
            led_toggle_status();
        if (should_run(BUTTON_POLL_RATE, loop_count))
        {
            buttons_poll();
            if (sleep_btn_held())
                low_power_mode();
            if (joy_btn_held())
                radio_transmit_dastardly();
        }
        if (should_run(RADIO_TRANSMIT_RATE, loop_count))
            radio_transmit_direction(throttle, heading);
        if (should_run(RADIO_RECEIVE_RATE, loop_count))
            if (radio_read_bumper())
                buzzer_bumper_hit();
        if (should_run(BATTERY_POLL_RATE, loop_count))
            if (is_low_battery())
                low_power_mode();
        if (should_run(LED_TAPE_UPDATE_RATE, loop_count))
            led_tape_update();

        loop_count = (loop_count % PACER_RATE) + 1;
    }
}