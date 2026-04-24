#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "math.h"

#include "../../pegues_lib/pegues_lib.h"
#include "../../pegues_lib/i2c.h"
#include "../../pegues_lib/ssd1306.h"

// SPI Defines
// We are going to use SPI 1, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define SPI_PORT spi1
#define PIN_MISO 12
#define PIN_CS 13
#define PIN_SCK 10
#define PIN_MOSI 11

#define PI 3.1415

#define SINE_FREQ 2.0
#define SINE_STEPS (int)(50.0 * SINE_FREQ)
uint16_t dac_sine[SINE_STEPS];
uint16_t dac_tri[SINE_STEPS];

struct repeating_timer dac_timer;

void init_spi()
{
    // SPI initialisation. This example will use SPI at 1MHz.
    // spi_init(SPI_PORT, 1000 * 1000);
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

void write_dac(uint16_t a_value, uint16_t b_value)
{
    cs_select(PIN_CS);
    uint8_t data[8];
    if (a_value <= 0b1111111111) // We've only got 10 bits available

    {
        data[0] = 0b0111 << 4 | ((a_value >> 6) & 0b00001111);
        data[1] = a_value << 2 & 0b11111100;

        spi_write_blocking(SPI_PORT, data, 2); // where data is a uint8_t array with length len
    }
    if (b_value <= 0b1111111111)
    {
        uint8_t data[8];
        data[0] = 0b1111 << 4 | ((b_value >> 6) & 0b00001111);
        data[1] = b_value << 2 & 0b11111100;

        spi_write_blocking(SPI_PORT, data, 2);
    }
    cs_deselect(PIN_CS);
}

bool dac_timer_callback(__unused struct repeating_timer *t)
{
    static int sine = 0;
    write_dac(dac_sine[sine], dac_tri[sine]);
    sine = (sine + 1) % SINE_STEPS;
}

int main()
{
    await_usb(0.5, true);
    init_i2c();
    ssd1306_setup(true);
    init_spi();
    printf("SPI inited\n");

    // Calculate sine wave points. They'll be between 0 and 0b1111111111.
    for (int i = 0; i < SINE_STEPS; i++)
    {
        dac_sine[i] = (-cos((float)i / (100.0) * (4.0 * PI)) + 1.0) / 2.0 * (float)0b1111111111;
        if (i < SINE_STEPS / 2)
        {
            dac_tri[i] = 2.0 * (float)i * (float)0b1111111111 / (float)SINE_STEPS;
        }
        else
        {
            dac_tri[i] = 2.0 * (float)0b1111111111 * (1 - (float)i / (float)SINE_STEPS);
        }
        printf("%d\n", dac_tri[i]);
    }

    char message[30];
    char data[30];

    sprintf(message, "Starting");
    ssd1306_write(0, 0, message);
    ssd1306_update();
    sleep_ms(100);
    add_repeating_timer_ms(-10, dac_timer_callback, NULL, &dac_timer);

    while (true)
    {

        breathe_led(0);
        sleep_ms(10);
    }
}
