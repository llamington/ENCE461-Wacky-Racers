PERIPHERALS = pit twi adc pwm
DRIVERS = pacer mpu9250 panic usb_serial ring led button piezo nrf24 ledtape
TARGET = hat.bin

SRC = hat.c radio.c imu.c led.c control.c joystick.c leds.c buttons.c buzzer.c power.c mmelody.c

# Use low level of optimisation to simplify debugging
OPT = -O0

BOARD = hat


INCLUDES += -I../../mmculib
VPATH += ../../mmculib

include ../../boards/board.mk


