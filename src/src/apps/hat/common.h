#ifndef COMMON_H
#define COMMON_H

#define PACER_RATE 1000 // pacer rate in Hz
#define DIRECTION_POLL_RATE 100
#define DIRECTION_READ_RATE 10
#define BUTTON_POLL_RATE 100
#define CH_POLL_RATE 50
#define CONTROL_METHOD_RATE 50
#define LED_FLASH_RATE 10
#define LED_FLASH_RATE_CALIB 2
#define BUTTON_POLL_RATE 100
#define RADIO_TRANSMIT_RATE 10
#define RADIO_RECEIVE_RATE 100
#define BATTERY_POLL_RATE 1
#define BUZZER_UPDATE_RATE 1000
#define LED_TAPE_UPDATE_RATE 25

/** Checks whether a task should run */
#define should_run(task_rate, counter) ((counter) % (PACER_RATE / (task_rate)) == 0)

/** Maps one numeric range to another */
#define map_range(a1, a2, b1, b2, s) ((b1) + ((s) - (a1)) * ((b2) - (b1)) / ((a2) - (a1)))

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define ERROR_CODE_IMU_TWI 1
#define ERROR_CODE_IMU 2
#define ERROR_CODE_JOY_ADC 3
#define ERROR_CODE_SPI 4
#define ERROR_CODE_RADIO 5
#define ERROR_CODE_BAT_ADC 6
#define ERROR_CODE_CH_SET 7

#endif