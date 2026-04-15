#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"

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

    i2c_init(I2C_PORT, 400 * 1000);

    ssd1306_setup(true);
    printf("SSD1306 display setup complete\n");
}

int main()
{
    await_usb(0, true);
    init_oled();
    adc_init();
    adc_select_input(0);
    int i = 15;
    char message[50];

    while (true)
    {
        breathe_led(0);

        sprintf(message, "ADC0 = %d", i++);
        ssd1306_drawString(0, 0, message);
        ssd1306_fps();
        ssd1306_update();
        i++;
        // sleep_ms(10);
    }
}