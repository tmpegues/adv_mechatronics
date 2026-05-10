#include <stdio.h>
#include "pico/stdlib.h"
#include "math.h"

#include "../../pegues_lib/pegues_lib.h"
#include "../../pegues_lib/i2c.h"
#include "../../pegues_lib/ssd1306.h"
#include "../../pegues_lib/spi.h"

#define DAC_CS 13
#define RAM_CS 14

#define PI 3.1415

#define SINE_STEPS 1000
uint8_t dac_sine[SINE_STEPS];

struct repeating_timer dac_timer;

union FloatInt
{
    float f;
    uint32_t i;
};

uint8_t ram_sine[SINE_STEPS * 4 + 2]; // *4 because I'm storing 32 bit floats, +2 to add dac instruction and address

bool dac_timer_callback(__unused struct repeating_timer *t)
{
    static int sine = 0;
    write_dac(DAC_CS, dac_sine[sine], -1);
    // printf("%f\n",dac_sine[sine]);
    sine = (sine + 1) % SINE_STEPS;
}

int main()
{
    await_usb(0.5, true);
    init_i2c();
    ssd1306_setup(true);
    init_spi();
    printf("SPI inited\n");
    init_ram(RAM_CS);

    ram_sine[0] = 0b00000010;
    ram_sine[1] = 0b0;

    // Calculate sine wave points. They'll be floats between 0 and 3.3.
    for (int i = 0; i < SINE_STEPS; i++)
    {
        int j = 4 * i + 2; // *4 because 4 byte float, +2 because those indexes are already reserved.
        union FloatInt value;
        value.f = (float)((-cos((float)i / (1000.0) * (2.0 * PI)) + 1.0) / 2.0 * 3.3);
        for (int k = 0; k < 4; k++) // <4 because 4 byte float, same as above
        {
            uint8_t bits = value.i >> ((8 * (3 - k)) & 0xFF); // 8*(3-k) so we start with >> 24, then >> 16, ...
            ram_sine[j + k] = bits;
            // ram_write_one(RAM_CS, j+k, bits);
        }
    }

    // Store values in ram, sequential load
    cs_select(RAM_CS);
    spi_write_blocking(spi1, ram_sine, 4*SINE_STEPS + 2);
    cs_deselect(RAM_CS);

    char message[30];
    char data[30];

    sprintf(message, "Starting");
    ssd1306_write(0, 0, message);
    ssd1306_update();
    sleep_ms(100);
    add_repeating_timer_ms(-10, dac_timer_callback, NULL, &dac_timer);

    sprintf(message, "Looping");
    ssd1306_write(0, 0, message);
    ssd1306_update();
    while (true)
    {
        breathe_led(0);
        sleep_ms(10);
    }
}
