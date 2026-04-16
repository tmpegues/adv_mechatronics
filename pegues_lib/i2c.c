#include "i2c.h"
#include "hardware/i2c.h"
#include <stdio.h>

#define I2C_SDA 4 // Pin 6
#define I2C_SCL 5 // Pin 7

void init_i2c()
{
    stdio_init_all();
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);

    i2c_init(i2c_default, 400 * 1000);
}

void write_i2c(unsigned char address, unsigned char reg, unsigned char value)
{
    unsigned char buf[2] = {reg, value};
    i2c_write_blocking(i2c_default, address, buf, 2, false);
}

unsigned char read_i2c(unsigned char address, unsigned char reg, unsigned char length)
{
    unsigned char buf;
    i2c_write_blocking(i2c_default, address, &reg, 1, true);      // true to keep host control of bus
    i2c_read_blocking(i2c_default, address, &buf, length, false); // false - finished with bus
    // printf("All pins: %d\n", buf);
    return buf;
}