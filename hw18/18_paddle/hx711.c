// init and read functions for the HX711 load cell amplifier (bit-banged)

#include "hx711.h"

#define HX711_SCK 16
#define HX711_DT 17
#define HX711_PULSE 1 // us

void init_hx711()
{
    gpio_init(HX711_SCK);
    gpio_set_dir(HX711_SCK, GPIO_OUT);

    gpio_init(HX711_DT);
    gpio_set_dir(HX711_DT, GPIO_IN);

    gpio_put(HX711_SCK, 1);
    sleep_us(HX711_PULSE);
    gpio_put(HX711_SCK, 0);
    sleep_us(HX711_PULSE);
}

int32_t read_hx711()
{
    gpio_put(HX711_SCK, 0);

    // DT goes low when a sample is ready
    while (gpio_get(HX711_DT) == 1)
    {
    }

    uint32_t raw = 0;
    for (int i = 0; i < 24; i++) // pulse 24 times, reading a bit after each rising edge
    {
        gpio_put(HX711_SCK, 1);
        sleep_us(HX711_PULSE);

        raw |= gpio_get(HX711_DT) << (23 - i);

        gpio_put(HX711_SCK, 0);
        sleep_us(HX711_PULSE);
    }

    // sign-extend 24-bit two's complement to 32-bit signed int
    if (raw & 0x800000)
    {
        raw |= 0xFF000000;
    }

    // 25th pulse selects gain 128 for the next read
    gpio_put(HX711_SCK, 1);
    sleep_us(HX711_PULSE);
    gpio_put(HX711_SCK, 0);

    return (int32_t)raw;
}
