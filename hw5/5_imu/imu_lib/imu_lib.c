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
#define ACCEL_XOUT_H 0x3B
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

int ACC_X = 0;
int ACC_Y = 0;
int ACC_Z = 0;

int GYR_X = 0;
int GYR_Y = 0;
int GYR_Z = 0;

int TEMP = 0;

bool init_imu(void)
{
    // To turn on the chip, write 0x00 to the PWR_MGMT_1 register
    unsigned char buf[2];
    write_i2c(ADDR, PWR_MGMT_1, 0x00);

    // Check correct by reading from WHO_AM_I
    unsigned char who = read_i2c(ADDR, WHO_AM_I, 1);
    if (who != 0x68)
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
    printf("reading all\n");
    unsigned char all_data = read_i2c(ADDR, ACCEL_XOUT_H, 14);

    // uint8_t all_data[14];
    // i2c_write_blocking(i2c_default, ADDR, ACCEL_XOUT_H, 1, true);      // true to keep host control of bus
    // i2c_read_blocking(i2c_default, ADDR, &all_data, 14, false);        // false - finished with bus

    printf("read all\n");

    // ACC_X = all_data & 0b1111111111111111;
    // all_data >> 16;
    // ACC_Y = all_data & 0b1111111111111111;
    // all_data >> 16;
    // ACC_Z = all_data & 0b1111111111111111;
    // all_data >> 16;
    // TEMP = all_data & 0b1111111111111111;
    // all_data >> 16;
    // GYR_X = all_data & 0b1111111111111111;
    // all_data >> 16;
    // GYR_Y = all_data & 0b1111111111111111;
    // all_data >> 16;
    // GYR_Z = all_data & 0b1111111111111111;
}



float acc_x(void)
{
    return ACC_X * 0.000061;
}
float acc_y(void)
{
    return ACC_Y * 0.000061;
}
float acc_z(void)
{
    return ACC_Z * 0.000061;
}