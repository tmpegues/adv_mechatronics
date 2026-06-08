// init and read functions for the AS5600 magnetic encoder

#include "as5600.h"

#define AS5600_ADDR 0x36 // I2C address
#define AS5600_REG_RAWANGLE 0x0C // raw angle register, 12 bit, 2 bytes

#define SDA_PIN 4
#define SCL_PIN 5
#define I2C_INST i2c0

void init_as5600(){
    // init I2C on GP4 (SDA) and GP5 (SCL) on I2C0
    i2c_init(I2C_INST, 400 * 1000); // baud of 400kHz
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);
}

// read the 12 bit raw angle (0 to 4095)
uint16_t read_as5600(){
    uint8_t reg = AS5600_REG_RAWANGLE;
    uint8_t buf[2];
    i2c_write_blocking(I2C_INST, AS5600_ADDR, &reg, 1, true); // true to keep host control of bus
    i2c_read_blocking(I2C_INST, AS5600_ADDR, buf, 2, false); // false - finished with bus
    return ((buf[0] << 8) | buf[1]) & 0x0FFF;
}
