#include <stdio.h>
#include "pico/stdlib.h"
#include "imu_lib.h"
#include "../../pegues_lib/i2c.h"

#define ADDR 0b1101000

// config registers
#define CONFIG 0x1A
#define GYRO_CONFIG 0x1B
#define ACCEL_CONFIG 0x1C
#define PWR_MGMT_1 0x6B
#define PWR_MGMT_2 0x6C
// sensor data registers:
#define ACCEL_XOUT_H _u(0x3B)
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
#define TEMP_OUT_H 0x41
#define TEMP_OUT_L 0x42
#define GYRO_XOUT_H 0x43
#define GYRO_XOUT_L 0x44
#define GYRO_YOUT_H 0x45
#define GYRO_YOUT_L 0x46
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48
#define WHO_AM_I 0x75

#define ACCEL_CONV 0.000061
#define GYRO_CONV 0.007630

int16_t ACC_X = 0;
int16_t ACC_Y = 0;
int16_t ACC_Z = 0;

int16_t GYR_X = 0;
int16_t GYR_Y = 0;
int16_t GYR_Z = 0;

int16_t TEMP = 0;

bool init_imu(void)
{
    // To turn on the chip, write 0x00 to the PWR_MGMT_1 register
    unsigned char buf[2];
    write_i2c(ADDR, PWR_MGMT_1, 0x00);

    // Check correct by reading from WHO_AM_I
    unsigned char who = read_i2c(ADDR, WHO_AM_I, 1);
    if (who != ADDR)
    {
        return false;
    }
    printf("Who: 0x%x\n", who);
    // To enable the accelerometer, write to the ACCEL_CONFIG register.
    // Set the sensitivity to plus minus 2g.
    // 0b00 sets sensitivity to 2g, shift by 3 to put it on bits 4 and 3
    write_i2c(ADDR, ACCEL_CONFIG, 0b0000000 | 0b00 << 3);
    // To enable the gyroscope, write to the GYRO_CONFIG register. Set the sensitivity to plus minus 2000 dps.
    // 0b11 sets sensitivity to 2000 dps, << 3 to put it on bits 4 and 3
    write_i2c(ADDR, GYRO_CONFIG, 0b0000000 | 0b11 << 3);

    return true;
}

void read_all(void)
{
    uint8_t reg = ACCEL_XOUT_H;
    uint8_t all_data[14];
    i2c_write_blocking(i2c_default, ADDR, &reg, 1, true);
    i2c_read_blocking(i2c_default, ADDR, all_data, 14, false);

    ACC_X = all_data[0] << 8 | all_data[1];
    ACC_Y = all_data[2] << 8 | all_data[3];
    ACC_Z = all_data[4] << 8 | all_data[5];
    TEMP = all_data[6] << 8 | all_data[7];
    GYR_X = all_data[8] << 8 | all_data[9];
    GYR_Y = all_data[10] << 8 | all_data[11];
    GYR_Z = all_data[12] << 8 | all_data[13];
}

float acc_x(void)
{
    return ACC_X * ACCEL_CONV;
}
float acc_y(void)
{
    return ACC_Y * ACCEL_CONV;
}
float acc_z(void)
{
    return ACC_Z * ACCEL_CONV;
}

float gyr_x(void)
{
    return GYR_X * GYRO_CONV;
}

float gyr_y(void)
{
    return GYR_Y * GYRO_CONV;
}

float gyr_z(void)
{
    return GYR_Z * GYRO_CONV;
}