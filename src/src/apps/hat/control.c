#include "control.h"
#include "imu.h"
#include "pio.h"
#include "joystick.h"

static control_t control_method;

/** Initialises the direction control */
void control_init(void)
{
    pio_config_set(CONTROL_METHOD_SELECT_PIO, PIO_PULLUP);
    imu_init();
    joystick_init();
}

/** Polls for the currently selected control method */
void poll_control_method(void)
{
    bool pio_state = pio_input_get(CONTROL_METHOD_SELECT_PIO);
    control_method = pio_state ? CONTROL_IMU : CONTROL_JOYSTICK;
}

/** Gets the direction specified by either the IMU or the joystick */
void read_direction(int8_t *throttle, int8_t *heading)
{
    switch (control_method)
    {
    case CONTROL_IMU:
        imu_read_direction(throttle, heading);
        break;
    case CONTROL_JOYSTICK:
        joystick_read_direction(throttle, heading);
        break;
    }
}

/** Polls for the current direction from either the IMU or the joystick */
void poll_direction(void)
{
    switch (control_method)
    {
    case CONTROL_IMU:
        imu_poll_direction();
        break;
    case CONTROL_JOYSTICK:
        joystick_poll_direction();
        break;
    }
}

void control_shutdown(void)
{
    joystick_shutdown();
    imu_shutdown();
}