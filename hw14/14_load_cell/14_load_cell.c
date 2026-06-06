#include <stdio.h>
#include "pico/stdlib.h"
#include "../../pegues_lib/pegues_lib.h"

#define LOAD_SCK 16
#define LOAD_DT 17



int main()
{
    await_usb(0, true);
    breathe_led(0);

    gpio_init(LOAD_SCK);
    gpio_set_dir(LOAD_SCK, GPIO_OUT);

    gpio_init(LOAD_DT);
    gpio_set_dir(LOAD_DT, GPIO_IN);

    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
