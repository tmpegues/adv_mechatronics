#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "pico/cyw43_arch.h"

#include "../../pegues_lib/pegues_lib.h"
#include "../../pegues_lib/i2c.h"
#include "../../pegues_lib/ssd1306.h"

// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define SPI_PORT spi1
#define PIN_MISO 8
#define PIN_CS 9
#define PIN_SCK 6
#define PIN_MOSI 7

void init_spi()
{
    // SPI initialisation. This example will use SPI at 1MHz.
    spi_init(SPI_PORT, 1000 * 1000);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_CS, GPIO_FUNC_SIO);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);
    // For more examples of SPI use see https://github.com/raspberrypi/pico-examples/tree/master/spi
}

int main()
{
    await_usb(0, true);
    stdio_init_all();

    // init_led();
    breathe_led(-1);

    init_i2c();
    // ssd1306_setup(true);
    // unsigned char oled_message[50];
    // sprintf(oled_message, "OLED on");
    // ssd1306_drawString(0, 0, oled_message);

    // // init_spi();
    // sprintf(oled_message, "SPI on");
    // ssd1306_drawString(0, 8, oled_message);

    while (true)
    {
        breathe_led(-1);
        printf("Hello, world!\n");
        sleep_ms(10);
    }
}
