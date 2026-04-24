#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

#include "../../pegues_lib/pegues_lib.h"
#include "../../pegues_lib/i2c.h"
#include "../../pegues_lib/ssd1306.h"

// SPI Defines
// We are going to use SPI 1, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define SPI_PORT spi1
#define PIN_MISO 12
#define PIN_CS 17
#define PIN_SCK 10
#define PIN_MOSI 11

void init_spi()
{
    // SPI initialisation. This example will use SPI at 1MHz.
    // spi_init(SPI_PORT, 1000 * 1000);
    spi_init(SPI_PORT, 1000);

    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_CS, GPIO_FUNC_SIO);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);
    // For more examples of SPI use see https://github.com/raspberrypi/pico-examples/tree/master/spi
}

static inline void cs_select(uint cs_pin)
{
    asm volatile("nop \n nop \n nop"); // FIXME
    gpio_put(cs_pin, 0);
    asm volatile("nop \n nop \n nop"); // FIXME
}

static inline void cs_deselect(uint cs_pin)
{
    asm volatile("nop \n nop \n nop"); // FIXME
    gpio_put(cs_pin, 1);
    asm volatile("nop \n nop \n nop"); // FIXME
}

void write_dac(bool pin_a, uint16_t value)
{
    // We've only got 10 bits available

    if (value <= 0b1111111111)
    {
        uint8_t data[8];
        // data[0] = !pin_a << 7 | 0b111 << 4 | ((value >> 6) & 0b00001111);
        // data[1] = value << 2 & 0b11111100;
        data[0] = 0b01111111;
        data[1] = 0b11111100;
        // data[1] = value << 2 & 0b11111100;
        printf("data0, data1: %x %x\n", data[0], data[1]);

        cs_select(PIN_CS);
        spi_write_blocking(SPI_PORT, data, 2); // where data is a uint8_t array with length len
        cs_deselect(PIN_CS);
    }
}

int main()
{
    await_usb(.1, true);
    init_i2c();
    ssd1306_setup(true);
    init_spi();
    printf("SPI inited\n");
    // printf("Sending\n\n"); // Indicates to the python plotter that data will start being sent

    // // Calculate sine wave points
    // float sine_freq = 2;
    // float dac_freq = 50 * sine_freq;

    char message[30];
    char data[30];

    sprintf(message, "Starting");
    ssd1306_write(0, 0, message);
    ssd1306_update();
    int i = 0;
    sleep_ms(100);
    uint16_t value = 0;

    while (true)
    {
        if (i % 2 == 0)
        {
            value = 0b1111111111;
        }
        else
        {
            value = 1;
        }
        write_dac(false, value);

        ssd1306_clear();
        sprintf(message, "loop %d\n", i);
        printf(message);

        ssd1306_write(0, 0, message);

        ssd1306_update();
        breathe_led(0);
        sleep_ms(100);
        i++;
    }
}
