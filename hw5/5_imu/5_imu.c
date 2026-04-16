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

    float ax = 0;
    float ay = 0;
    float az = 0;

    while (true)
    {
        read_all();
        char message[30];
        // The 2 lines below are for plotting with python
        // printf("A\n%+2.2f %+2.2f %+2.2f\n", acc_x(), acc_y(), acc_z());
        // printf("G\n%+2.2f %+2.2f %+2.2f\n", gyr_x(), gyr_y(), gyr_z());

        // acc_x - is board tilted to the right
        // acc_y + is board standing up on flat end
        // acc_z + is board flat

        // ssd1306 is 128 x 32
        ssd1306_clear();

        // Draw x and y lines
        // Multiply by 1/2 of screen dimension to scale 1g to screen size
        // Add 1/2 of screen dimension to center the line
        float ax = acc_x();
        printf("%f\n", ax);
        int x_length = -1 * ax * 64;
        printf("%d\n", x_length);

        int y_length = acc_y() * 16;
        int i = 0;
        while (-64 <= i && i <= 128)
        {
            if ((x_length <= i && i <= 0) || (0 <= i && i <= x_length))
            {
                ssd1306_drawPixel(64 + i, 15, 1);
                ssd1306_drawPixel(64 + i, 16, 1);
            }
            (x_length >= 0) ? i++ : i--;
        }

        for (int i = 0; i <= 32; i++)
        {
            ssd1306_drawPixel(63, i-1, 1);
            ssd1306_drawPixel(64, i-1, 1);
        }

        ssd1306_update();
        // sleep_ms(100);
    }
}
