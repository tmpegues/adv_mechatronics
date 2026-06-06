#include <stdio.h>
#include "pico/stdlib.h"
#include "../../pegues_lib/pegues_lib.h"

int main()
{
    await_usb(0, true);
    breathe_led(0);

    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
