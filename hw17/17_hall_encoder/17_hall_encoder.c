#include <stdio.h>
#include "pico/stdlib.h"
#include "../../pegues_lib/pegues_lib.h"
#include "../../pegues_lib/i2c.h"

#define ENCODER_PIN 0
#define ENCODER_ADDR 0x36
#define ENCODER_CONF 0x08
#define ENCODER_ANGL 0x0C
uint8_t encoder_angl = ENCODER_ANGL;

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

    char buffer;
    while (true)
    {

        printf("(%d)\n", read_encoder());

        sleep_ms(100);
    }
}
