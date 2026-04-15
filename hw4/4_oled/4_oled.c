#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "../../pegues_lib/pegues_lib.h"
#include "ssd1306/ssd1306.h"

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

    ssd1306_setup();
    printf("SSD1306 display setup complete\n");
}

int main()
{
    await_usb(0, true);
    init_oled();
    ssd1306_fill();
    ssd1306_drawLetter(0, 0, 'b');

    while (true)
    {
        breathe_led(0);
        ssd1306_drawPixel(0, 0, 1);
        ssd1306_drawPixel(127, 31, 1);

        ssd1306_update();
        sleep_ms(100);

        breathe_led(0);
        ssd1306_drawPixel(0, 0, 0);
        ssd1306_drawPixel(127, 31, 0);
        ssd1306_update();
        sleep_ms(100);
    }
}