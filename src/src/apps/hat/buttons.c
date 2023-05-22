#include "buttons.h"
#include "common.h"
#include "button.h"

#define BTN_HOLD_COUNT 10

/* Define button configuration.  */
static const button_cfg_t sleep_btn_cfg =
    {
        .pio = BUTTON_PIO};

static const button_cfg_t joy_btn_cfg =
    {
        .pio = JOYSTICK_BUTTON_PIO};

static button_t sleep_btn;
static button_t joy_btn;

void buttons_init(void)
{
    sleep_btn = button_init(&sleep_btn_cfg);
    joy_btn = button_init(&joy_btn_cfg);

    button_poll_count_set(BUTTON_POLL_COUNT(BUTTON_POLL_RATE));
}

void buttons_poll(void)
{
    button_poll(sleep_btn);
    button_poll(joy_btn);
}

bool sleep_btn_pushed(void)
{
    return button_pushed_p(sleep_btn);
}

bool sleep_btn_held(void)
{
    return button_held_p(sleep_btn, BTN_HOLD_COUNT);
}

bool joy_btn_pushed(void)
{
    return button_pushed_p(joy_btn);
}

bool joy_btn_held(void)
{
    return button_held_p(joy_btn, BTN_HOLD_COUNT);
}