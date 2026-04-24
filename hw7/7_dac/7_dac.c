#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

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
    await_usb(0, true); // Remove in final version
    init_i2c();
    ssd1306_setup(true);
    init_spi();
    printf("SPI inited\n");
    // printf("Sending\n\n"); // Indicates to the python plotter that data will start being sent
    // sleep_ms(100);

    // char message[30];

    while (true)
    {
        ssd1306_clear();


        ssd1306_update();
        breathe_led(0);
    }
}
