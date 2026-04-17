#include <stdio.h>
#include "pico/stdlib.h"

#include "../../pegues_lib/pegues_lib.h"
#include "../../pegues_lib/i2c.h"
#include "../../pegues_lib/ssd1306.h"
#include "../../pegues_lib/imu_lib.h"

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

    float ax = 0;
    float ay = 0;
    float az = 0;
    char message[30];

    while (true)
    {
        read_all();
        ax = acc_x();
        ay = acc_y();
        az = acc_z();

        // The 2 lines below are for plotting with python
        // printf("A\n%+2.2f %+2.2f %+2.2f\n", ax, ay, az);
        // printf("G\n%+2.2f %+2.2f %+2.2f\n", gyr_x(), gyr_y(), gyr_z());

        // ssd1306 is 128 x 32
        ssd1306_clear();
        // acc_x - is board tilted to the right
        // acc_y + is board standing up on flat end
        // acc_z + is board flat
        // ax *= -64;
        // ay *= 64;
        // az *= 64; // It isn't actually necessary to scale z, but it gets cast to an int
        // draw_axes(ax, ay, az);
        draw_vector(ax, ay, az);

        ssd1306_update();
        breathe_led(0);
    }
}
