#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define PWMDIV 3000
#define WRAP 1000
#define SERVO 16

#define PWM_MIN 0
#define PWM_MAX 100

void init_pwm()
{
    gpio_set_function(SERVO, GPIO_FUNC_PWM);
    uint slice_num1 = pwm_gpio_to_slice_num(SERVO);
    pwm_set_clkdiv(slice_num1, PWMDIV);
    pwm_set_wrap(slice_num1, WRAP);
    pwm_set_enabled(slice_num1, true);
    pwm_set_gpio_level(SERVO, 0);
}

float deg_to_pwm(float angle)
{
    float angle_prop = (angle / 180.0);
    float pwm_range = ((float)PWM_MAX - (float)PWM_MIN);
    float duty = angle_prop*pwm_range + PWM_MIN;
    printf("angle prop: %f, pwm_range: %f, duty: %f, angle: %f\n", angle_prop, pwm_range, duty, angle);
    return duty;
}

int main()
{
    stdio_init_all();
    init_pwm();

    while (true)
    {
        for (int i = 0; i <= 360; i++)
        {
            printf("\n");
            float duty;
            printf("i: %d\n", i);
            if (i <= 180)
            {
                duty = deg_to_pwm(i);
            }
            else
            {
                duty = deg_to_pwm(360 - i);
            }
            printf("duty: %f\n",duty);
            pwm_set_gpio_level(SERVO, duty);
            sleep_ms(100);
        }
    }
}
