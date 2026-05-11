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
#define ARRAY_OFFSET 3                           // This will hold the first n slots in the array for the instruction and address
uint8_t ram_sine[SINE_STEPS * 2 + ARRAY_OFFSET]; // *4 because I'm storing 16 bit values

struct repeating_timer dac_timer;

union FloatInt
{
    float f;
    uint32_t i;
};

bool dac_timer_callback(__unused struct repeating_timer *t)
{
    static uint16_t sine_index = 0;
    static union FloatInt sine_value;
    sine_value.i = ram_read_16(RAM_CS, sine_index * 2 + 2);

    write_dac(DAC_CS, sine_value.f, -1);
    sine_index = (sine_index + 1) % SINE_STEPS;
}

int main()
{
    await_usb(0, true);
    init_i2c();
    ssd1306_setup(true);
    init_spi();
    printf("SPI inited\n");
    init_ram(RAM_CS);

    ram_sine[0] = 0b00000010;
    ram_sine[1] = 0b0;
    ram_sine[2] = 0b0;

    // Calculate sine wave points. They'll be floats between 0 and 3.3.
    // Nevermind, they're supposed to be formatted for the DAC (uint16 <= 0b1111111111)
    for (int i = 0; i < SINE_STEPS; i++)
    {
        int j = 2 * i + ARRAY_OFFSET; // *2 because 2 byte values
        uint16_t value;
        value = (-cos((float)i / (1000.0) * (4.0 * PI)) + 1.0) / 2.0 * 0b1111111111;
        for (int k = 0; k < 2; k++) // <2 because 2 byte value, same as above
        {
            uint8_t bits = value >> ((8 * (1 - k)) & 0xFF); // 8*(1-k) so we start with >> 8 then >> 0
            ram_sine[j + k] = bits;
            if (k == 0)
            {
                printf("j+k, bits: %4d, %8b,", j + k, bits);
            }
            else
            {
                printf("%8b (j+k = %4d)\n", bits, j + k);
            }
        }
    }

    // Store values in ram, sequential load
    printf("ram_sine 0, 1, 2: %b, %b, %b\n", ram_sine[0], ram_sine[1], ram_sine[2]);
    cs_select(RAM_CS);
    spi_write_blocking(spi1, ram_sine, 2 * SINE_STEPS + ARRAY_OFFSET);
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
