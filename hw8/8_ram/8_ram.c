#include <stdio.h>
#include "pico/stdlib.h"
#include "math.h"

#include "../../pegues_lib/pegues_lib.h"
#include "../../pegues_lib/i2c.h"
#include "../../pegues_lib/ssd1306.h"
#include "../../pegues_lib/spi.h"


#define DAC_CS 13


#define PI 3.1415

#define SINE_STEPS 1000
uint16_t dac_sine[SINE_STEPS];
uint16_t dac_tri[SINE_STEPS];

struct repeating_timer dac_timer;


bool dac_timer_callback(__unused struct repeating_timer *t)
{
    static int sine = 0;
    write_dac(DAC_CS, dac_sine[sine], dac_tri[sine]);
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
