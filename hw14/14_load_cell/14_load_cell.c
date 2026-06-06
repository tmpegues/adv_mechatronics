#include <stdio.h>
#include "pico/stdlib.h"
#include "../../pegues_lib/pegues_lib.h"

#define LOAD_SCK 16
#define LOAD_DT 17
#define PULSE 10

int main()
{
    await_usb(0, true);
    breathe_led(0);

    gpio_init(LOAD_SCK);
    gpio_set_dir(LOAD_SCK, GPIO_OUT);

    gpio_init(LOAD_DT);
    gpio_set_dir(LOAD_DT, GPIO_IN);

    gpio_put(LOAD_SCK, 0);

    while (true)
    {
        gpio_put(LOAD_SCK, 0);

        if (gpio_get(LOAD_DT) == 1)
        {
            printf("Reading Load\n");
        }
        else
        {
            uint32_t raw_load = 0;
            for (int i = 0; i < 24; i++) // Pulse 24 times for reading
            {
                gpio_put(LOAD_SCK, 1);
                sleep_ms(PULSE);


                raw_load |= gpio_get(LOAD_DT) << 24 - i;
                // bool read = gpio_get(LOAD_DT);
                printf("loop: %2d, %b\n", i, raw_load);
                // printf("loop: %d, %d\n", i, read);

                gpio_put(LOAD_SCK, 0);
                sleep_ms(PULSE);
            }
            printf("raw: %d\n", raw_load);
            if (raw_load & 0x800000)
            {
                raw_load |= 0xFF000000;
            }
            printf("moved: %d\n", raw_load);

            // Pulse 1 more time to set gain 128
            gpio_put(LOAD_SCK, 1);
            sleep_ms(PULSE);
            gpio_put(LOAD_SCK, 0);
        }

        sleep_ms(10);
    }
}

// X Solder the HX711 to the force sensor, 1k resistors, header pins, and set to sample at 80Hz.

// X Write a function to initialize 2 GPIO pins for SCK (output) and DT (input).

// Write a function to read out the 24 bits from the sensor, waiting until the DT pin is low,
// then blinking SCK 25 times, reading DT the first 24 times, and shifting the data and converting to a signed int.

// Make a Pico project that initializes the HX711, and waits for the computer to send a number
// of samples to collect. As you collect the data, save the raw value, an IIR filtered value,
// and what time it is in milliseconds. Print all there back after all the data is collected.

// Write a Python script that sends a number of data points to collect, and then reads all the
// data back, and plots the raw and filtered data vs time, and takes an FFT of both. The Nyquist
// frequency should be about 40Hz. When touching the sensor, I saw noise at 25-30Hz, so try
// to reduce that with the IIR. Upload the code to a folder called HW14, and submit images of the data and FFT to Canvas.
