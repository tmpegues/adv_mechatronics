#include <stdio.h>
#include "pico/stdlib.h"

#include "../../pegues_lib/pegues_lib.h"
#include "../../pegues_lib/i2c.h"
#include "../../pegues_lib/ssd1306.h"
#include "imu_lib/imu_lib.h"

int main()
{
    await_usb(0, true);
    init_i2c();
    int i = 0;
    while (!init_imu())
    {
        printf("Init IMU loop: %d\n", i);
    }
    printf("IMU setup complete\n");
    ssd1306_setup(true);
    printf("Sending\n\n"); // Indicates to the python plotter that data will start being sent
    sleep_ms(100);
    while (true)
    {
        read_all();
        char message[30];

        sprintf(message, "A: %+2.2f, %+2.2f, %+2.2f\n", acc_x(), acc_y(), acc_z());
        ssd1306_drawString(0, 0, message);
        sprintf(message, "G: %+2.2f, %+2.2f, %+2.2f\n", gyr_x(), gyr_y(), gyr_z());
        ssd1306_drawString(0, 8, message);
        printf("A\n%+2.2f %+2.2f %+2.2f\n", acc_x(), acc_y(), acc_z());
        printf("G\n%+2.2f %+2.2f %+2.2f\n", gyr_x(), gyr_y(), gyr_z());

        ssd1306_update();
        // sleep_ms(100);
    }
}
