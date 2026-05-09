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
float dac_sine[SINE_STEPS];

struct repeating_timer dac_timer;

union FloatInt
{
    float f;
    uint32_t i;
};

union FloatInt ram_sine[SINE_STEPS];

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

    // Calculate sine wave points. They'll be floats between 0 and 3.3.
    for (int i = 0; i < SINE_STEPS; i++)
    {
        union FloatInt value;
        value.f = (float)((-cos((float)i / (1000.0) * (2.0 * PI)) + 1.0) / 2.0 * 3.3);
        ram_sine[i] = value;
        printf("ram sine: %f\n", ram_sine[i].f);
    }

    char message[30];
    char data[30];

    sprintf(message, "Starting");
    ssd1306_write(0, 0, message);
    ssd1306_update();
    sleep_ms(100);
    add_repeating_timer_ms(-10, dac_timer_callback, NULL, &dac_timer);
    int i = 0;

    while (true)
    {
        breathe_led(0);
        sleep_ms(10);
        printf("ram sine %d: %f\n", i, ram_sine[i].f);
        i = (i + 1) % SINE_STEPS;
    }
}
