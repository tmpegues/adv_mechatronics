#include <stdio.h>
#include "pico/stdlib.h"

#include "../../pegues_lib/pegues_lib.h"
#include "../../pegues_lib/i2c.h"
#include "../../pegues_lib/ssd1306.h"

int main()
{
    await_usb(0, true);
    init_i2c();
    ssd1306_setup(true);
    printf("Sending\n\n"); // Indicates to the python plotter that data will start being sent
    sleep_ms(100);

    char message[30];

    while (true)
    {
        ssd1306_clear();


        ssd1306_update();
        breathe_led(0);
    }
}
