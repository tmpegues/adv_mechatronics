#include <stdio.h>
#include "pico/stdlib.h"
#include "../../pegues_lib/pegues_lib.h"
#include "../../pegues_lib/i2c.h"

#define ENCODER_PIN 0
#define ENCODER_ADDR 0x36
#define ENCODER_CONF 0x08
#define ENCODER_ANGL 0x0C

#define LOAD_SCK 16
#define LOAD_DT 17
#define PULSE 1

uint8_t encoder_angl = ENCODER_ANGL;

void init_load_cell()
{
    gpio_init(LOAD_SCK);
    gpio_set_dir(LOAD_SCK, GPIO_OUT);

    gpio_init(LOAD_DT);
    gpio_set_dir(LOAD_DT, GPIO_IN);

    gpio_put(LOAD_SCK, 1);
    sleep_us(PULSE);
    gpio_put(LOAD_SCK, 0);
    sleep_us(PULSE);
}

int32_t read_load_cell()
{
    gpio_put(LOAD_SCK, 0);

    while (gpio_get(LOAD_DT) == 1)
    {
    }

    uint32_t raw_load = 0;
    for (int i = 0; i < 24; i++) // Pulse 24 times for reading
    {
        gpio_put(LOAD_SCK, 1);
        sleep_us(PULSE);

        raw_load |= gpio_get(LOAD_DT) << 23 - i;

        gpio_put(LOAD_SCK, 0);
        sleep_us(PULSE);
    }

    if (raw_load & 0x800000)
    {
        raw_load |= 0xFF000000;
    }

    // Pulse 1 more time to set gain 128
    gpio_put(LOAD_SCK, 1);
    sleep_us(PULSE);
    gpio_put(LOAD_SCK, 0);

    return raw_load;
}

uint16_t read_encoder()
{
    unsigned char buf[2];
    i2c_write_blocking(i2c_default, ENCODER_ADDR, &encoder_angl, 1, true); // true to keep host control of bus
    i2c_read_blocking(i2c_default, ENCODER_ADDR, buf, 2, false);           // false - finished with bus
    return buf[0] << 8 | buf[1];
}

int main()
{
    await_usb(0, true);
    breathe_led(0);

    init_i2c();
    write_i2c(ENCODER_ADDR, ENCODER_CONF, 0 & 0b00 << 4);

    init_load_cell();

    while (true)
    {

        printf("(%d, %d)\n", read_load_cell(), read_encoder());

        sleep_ms(100);
    }
}
