#ifndef IMU_H
#define IMU_H

#include <stdio.h>

typedef enum
{
    IMU_SUCCESS,
    IMU_UNREADY,
    IMU_FAILURE
} imu_state_t;

void imu_read_raw_accel(int16_t *raw_data);

void imu_init(void);

imu_state_t imu_poll_direction(void);

void imu_read_direction(int8_t *throttle, int8_t *heading);

void imu_z_angle(int16_t raw_accel[3], float *x_rad, float *y_rad);

void imu_shutdown(void);

#endif