#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "pegues_lib.h"

#define PWMDIV 60 // must be between 1-255
#define WRAP 50000
#define SERVO 16

#define DUTY_MIN 0.024 // 480 us (.024) from website (https://daniel.springwald.de/post/2023/RCServoCheatSheet)
#define DUTY_MAX 0.128 // 2500 us, (.125) from website
#define SWEEP_SLEEP 1
#define SWEEP_STEP 0.001

void init_pwm()
{   // 150 000 000 / PWMDIV / WRAP
    gpio_set_function(SERVO, GPIO_FUNC_PWM);
    uint slice_num1 = pwm_gpio_to_slice_num(SERVO);
    pwm_set_clkdiv(slice_num1, PWMDIV);
    pwm_set_wrap(slice_num1, WRAP);
    pwm_set_enabled(slice_num1, true);
    printf("PWM freq = %d\n", 150000000/PWMDIV/WRAP);
}

void servo_angle(int servo_pin, float angle)
{
    float angle_prop = (angle / 180.0);
    float pwm_range = (DUTY_MAX - DUTY_MIN);
    uint16_t pwm_value = (angle_prop * pwm_range + DUTY_MIN) * WRAP;
    printf("angle prop: %.2f, angle: %3.2f, pwm/wrap: %u/%d\n", angle_prop, angle, pwm_value, WRAP);
    pwm_set_gpio_level(servo_pin, pwm_value);
}

void pwm_sweep()
{
    static float duty = DUTY_MIN-SWEEP_STEP;
    static int sign = 1;
    duty += SWEEP_STEP * sign; // Increment by 1%
    if (duty > DUTY_MAX)
    {
        duty = DUTY_MAX-SWEEP_STEP;
        sign = -1;
        printf("Flip to descend\n");
        sleep_ms(1000);
    }
    else if (duty < DUTY_MIN)
    {
        duty = DUTY_MIN+SWEEP_STEP;
        sign = 1;
        printf("Flip to ascend\n");
        sleep_ms(1000);
    }

    uint16_t pwm_value = duty * WRAP;

    printf("Duty, PWM value, WRAP: %.4f, %5u, %d\n", duty, pwm_value, WRAP);
    pwm_set_gpio_level(SERVO, pwm_value);
    sleep_ms(SWEEP_SLEEP);
    toggle_led(0);
}

int main()
{
    stdio_init_all();
    int rc = init_led();
    await_usb();
    init_pwm();

    printf("Starting sweep\n");

    int sign = 1;
    while (true)
    {
        for (float angle = 0; angle < 180; angle += .1)
        {
            float actual_angle;
            if (sign > 0)
            {
                actual_angle = angle;
            }
            else if (sign < 0)
            {
                actual_angle = 180.0 - angle;
            }
            servo_angle(SERVO, actual_angle);
            sleep_ms(SWEEP_SLEEP);
        }
        sign *= -1;
    }
}
