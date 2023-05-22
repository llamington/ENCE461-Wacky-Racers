/** @file   target.h
    @author M. P. Hayes, UCECE
    @date   12 February 2018
    @brief
*/
#ifndef TARGET_H
#define TARGET_H

#include "mat91lib.h"

/* This is for the carhat (chart) board configured as a hat!  */

#define SERIAL_DATA

/* System clocks  */
#define F_XTAL 12.00e6
#define MCU_PLL_MUL 16
#define MCU_PLL_DIV 1

#define MCU_USB_DIV 2
/* 192 MHz  */
#define F_PLL (F_XTAL / MCU_PLL_DIV * MCU_PLL_MUL)
/* 96 MHz  */
#define F_CPU (F_PLL / 2)

/* TWI  */
#define TWI_TIMEOUT_US_DEFAULT 10000
#define MPU_ADDRESS 0x68

/* USB  */
//#define USB_VBUS_PIO PA5_PIO
#define USB_CURRENT_MA 500

/* ADC  */
#define BATTERY_ADC_CHANNEL ADC_CHANNEL_0
#define JOYSTICK_X_ADC_CHANNEL ADC_CHANNEL_5
#define JOYSTICK_Y_ADC_CHANNEL ADC_CHANNEL_4
#define JOYSTICK_BUTTON_PIO PB2_PIO

/* IMU  */
#define IMU_INT_PIO PA27_PIO

/* LEDs  */
#define LED_ERROR_PIO PA0_PIO
#define LED_STATUS_PIO PA1_PIO

/* General  */
#define APPENDAGE_PIO PA1_PIO
#define PIEZO_PWM_PIO PA2_PIO
#define CONTROL_METHOD_SELECT_PIO PA7_PIO

/* Button  */
#define BUTTON_PIO PA9_PIO

/* Radio  */
#define RADIO_CS_PIO PA11_PIO
#define RADIO_CE_PIO PA26_PIO
#define RADIO_IRQ_PIO PA15_PIO

/* LED tape  */
#define LEDTAPE_PIO PA8_PIO

/* Radio Channel Select */
#define CH0_PIO PA20_PIO
#define CH1_PIO PA23_PIO
#define CH2_PIO PA22_PIO
#define CH3_PIO PA19_PIO

#endif /* TARGET_H  */
