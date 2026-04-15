#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "../../pegues_lib/pegues_lib.h"
#include "../../ssd1306/ssd1306.h"

int main()
{
    await_usb(0, true);
    init_i2c();
    ssd1306_setup(true);

    while (true)
    {
        ssd1306_update();
    }
}
