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

    int i = 15;
    char message[50];
    sprintf(message, "my var = %d", i);
    ssd1306_drawString(20, 10, message); // draw starting at x=20,y=10

    while (true)
    {
        breathe_led(0);
        ssd1306_drawPixel(0, 0, 1);
        ssd1306_drawPixel(127, 31, 1);

        ssd1306_update();
        sleep_ms(1000);

        breathe_led(0);
        ssd1306_drawPixel(0, 0, 0);
        ssd1306_drawPixel(127, 31, 0);
        ssd1306_update();
        sleep_ms(1000);
    }
}