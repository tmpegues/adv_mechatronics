#include <stdio.h>
#include "pico/stdlib.h"
#include "../../pegues_lib/pegues_lib.h"

int main()
{
    // stdio_init_all();
    // int rc = init_led();
    await_usb(0, true);

    int sign = 1;
    while (true)
    {
        breathe_led(0);
        sleep_ms(10);
    }
}