#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "../../pegues_lib/pegues_lib.h"

#define I2C_PORT i2c_default
#define I2C_SDA 4 // Pin 6
#define I2C_SCL 5 // Pin 7

void init_oled()
{
    stdio_init_all();
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);

    // I2C Initialisation. Using it at 100Khz.
    i2c_init(I2C_PORT, 100 * 1000);

    // Use Nick's OLED init?
}

int main()
{
    await_usb(0, true);
    init_oled();
    while (true)
    {
        breathe_led(-1);
        sleep_ms(50);
    }
}
