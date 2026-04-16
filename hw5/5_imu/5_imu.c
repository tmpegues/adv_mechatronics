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

    while (true)
    {
        read_all();
        // printf("Acc x, y, z: %+2.2f, %+2.2f, %+2.2f\n", acc_x(), acc_y(), acc_z());
        printf("Gyr x, y, z: %+2.2f, %+2.2f, %+2.2f\n", acc_x(), acc_y(), acc_z());

        ssd1306_update();
        sleep_ms(100);
    }
}
