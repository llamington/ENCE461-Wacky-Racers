#include <stdlib.h>
#include "twi.h"
#include "mpu9250.h"
#include "panic.h"
#include "imu.h"
#include "common.h"
#include "ring.h"
#include "pacer.h"
#include "buttons.h"
#include "leds.h"
#include <math.h>

#define RING_BUF_SIZE 11
#define GRAVITY_RAW 16384  // Magnitude of gravity read from accelerometer
#define MAX_TILT 0.785     // Tilt in radians which results in maximum throttle
#define MAX_ROTATION 0.785 // Rotation of hat in radians which results in maximum heading

/** MPU config */
static twi_cfg_t mpu_twi_cfg =
    {
        .channel = TWI_CHANNEL_0,
        .period = TWI_PERIOD_DIVISOR(100000), // 100 kHz
        .slave_addr = 0};

static twi_t mpu_twi;

static mpu_t *mpu;

/** Ring buffers for accelerometer x, y, z values */
static ring_t *ring_buf_accel[3];

/** Initialize IMU */
void imu_init(void)
{
    // Initialise the TWI (I2C) bus for the MPU
    mpu_twi = twi_init(&mpu_twi_cfg);

    if (!mpu_twi)
        panic(LED_ERROR_PIO, ERROR_CODE_IMU_TWI);

    // Initialise the MPU9250 IMU
    mpu = mpu9250_init(mpu_twi, MPU_ADDRESS);

    if (!mpu)
        panic(LED_ERROR_PIO, ERROR_CODE_IMU);

    for (int i = 0; i < 3; i++)
        ring_buf_accel[i] = ring_init(NULL, NULL, RING_BUF_SIZE * sizeof(int16_t));
}

/** Polls for the current direction from the magnetometer */
imu_state_t imu_poll_direction(void)
{
    /* Read in the magnetometer data.  */
    if (!mpu9250_is_mag_ready(mpu))
        return IMU_UNREADY;

    int16_t imu_data[3];

    /** Read accelerometer data into ring buffer */
    bool status = mpu9250_read_accel(mpu, imu_data);

    ring_write_continuous(ring_buf_accel[0], &imu_data[0], sizeof(int16_t));
    ring_write_continuous(ring_buf_accel[1], &imu_data[1], sizeof(int16_t));
    ring_write_continuous(ring_buf_accel[2], &imu_data[2], sizeof(int16_t));

    return status
               ? IMU_SUCCESS
               : IMU_FAILURE;
}

/** Read the direction from the IMU */
void imu_read_direction(int8_t *throttle, int8_t *heading)
{
    int16_t raw_data[3];
    imu_read_raw_accel(raw_data);

    float x_rad, y_rad;
    imu_z_angle(raw_data, &x_rad, &y_rad);

    /** Compute throttle */
    float constrained_tilt = MIN(MAX_TILT, MAX(-MAX_TILT, x_rad));
    *throttle = (int8_t)map_range(MAX_TILT, -MAX_TILT, -127.0, 127.0, constrained_tilt);

    // /** Compute heading */
    float constrained_heading = MIN(MAX_ROTATION, MAX(-MAX_TILT, y_rad));
    *heading = (int8_t)map_range(-MAX_ROTATION, MAX_ROTATION, -127.0, 127.0, constrained_heading);

#ifdef SERIAL_DATA
    printf("heading: %d\n", *heading);
    printf("throttle: %d\n", *throttle);
#endif
}

/** Read the raw IMU Accelerometer data */
void imu_read_raw_accel(int16_t *raw_data)
{
    ring_size_t num_samples = ring_read_num(ring_buf_accel[0]) / sizeof(int16_t);

    int32_t x_avg = 0;
    int32_t y_avg = 0;
    int32_t z_avg = 0;

    int16_t temp = 0;

    for (uint8_t i = 0; i < num_samples; i++)
    {
        ring_read(ring_buf_accel[0], &temp, sizeof(int16_t));
        x_avg += temp;
        ring_read(ring_buf_accel[1], &temp, sizeof(int16_t));
        y_avg += temp;
        ring_read(ring_buf_accel[2], &temp, sizeof(int16_t));
        z_avg += temp;
    }

    x_avg /= num_samples;
    y_avg /= num_samples;
    z_avg /= num_samples;

    raw_data[0] = x_avg;
    raw_data[1] = y_avg;
    raw_data[2] = z_avg;
}

/** Angle of the IMU from flat */
void imu_z_angle(int16_t raw_accel[3], float *x_rad, float *y_rad)
{
    *x_rad = asin((float)raw_accel[0] / GRAVITY_RAW);
    *y_rad = asin((float)raw_accel[1] / GRAVITY_RAW);
}

void imu_shutdown(void)
{
    twi_shutdown(mpu_twi);
}