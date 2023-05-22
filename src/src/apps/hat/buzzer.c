#include "buzzer.h"
#include "piezo.h"
#include "piezo_beep.h"
#include "common.h"
#include "mmelody.h"
#include "pwm.h"
#include "panic.h"
#include "math.h"

#define TUNE_BPM 100
#define PWM_FREQ_HZ 100e3
#define DF1 "G/A/GF#E/"
#define DF2 "EF#G/A5/B4/A5/GF#E/ "
#define DF3 " D#EF#/G5/A4/G5/F#ED#/"
#define DUEL_FATES DF1 DF1 DF1 DF1 " " DF2 DF2 DF3 DF3 "/ "
#define MARIO_DEATH "B5F5/ F5E5D5C5E4E4C4"

static const piezo_cfg_t piezo_config =
    {
        .pio = PIEZO_PWM_PIO};

static piezo_t piezo;

static mmelody_obj_t mmelody_dev;

static mmelody_t mmelody;

static pwm_t buzzer_pwm;

static const char main_melody[] = DUEL_FATES ":";
static const char bumper_hit_melody[] = MARIO_DEATH "             <" DUEL_FATES ">";

static const pwm_cfg_t buz_pwm_cfg =
    {
        .pio = PIEZO_PWM_PIO,
        .period = PWM_PERIOD_DIVISOR(PWM_FREQ_HZ),
        .duty = PWM_DUTY_DIVISOR(PWM_FREQ_HZ, 50),
        .align = PWM_ALIGN_LEFT,
        .polarity = PWM_POLARITY_HIGH,
        .stop_state = PIO_OUTPUT_LOW};

void callback(void *data, uint8_t note, uint8_t volume)
{
    /** If below the valid note range */
    if (note < 60)
        pwm_stop(buzzer_pwm);
    else
    {
        pwm_start(buzzer_pwm);
        uint32_t frequency = 440.0 * pow(2.0, (note - 69.0) / 12.0);
        pwm_frequency_set(buzzer_pwm, frequency);
    }
}

void buzzer_init(void)
{
    piezo = piezo_init(&piezo_config);
    mmelody = mmelody_init(&mmelody_dev, BUZZER_UPDATE_RATE, callback, NULL);
    mmelody_speed_set(mmelody, 255);
    mmelody_play(mmelody, main_melody);

    buzzer_pwm = pwm_init(&buz_pwm_cfg);

    if (!buzzer_pwm)
        panic(LED_ERROR_PIO, 10);

    pwm_start(buzzer_pwm);
}

void buzzer_update(void)
{
    mmelody_update(mmelody);
}

void buzzer_beep(void)
{
    piezo_beep_long(piezo);
}

void buzzer_bumper_hit(void)
{
    mmelody_play(mmelody, bumper_hit_melody);
}