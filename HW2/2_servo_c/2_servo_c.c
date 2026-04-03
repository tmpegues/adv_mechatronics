#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "pegues_lib.h"


#define PWMDIV 3000
#define WRAP 1000
#define SERVO 16

#define DUTY_MIN 0
#define DUTY_MAX 10


void init_pwm()
{
    gpio_set_function(SERVO, GPIO_FUNC_PWM);
    uint slice_num1 = pwm_gpio_to_slice_num(SERVO);
    pwm_set_clkdiv(slice_num1, PWMDIV);
    pwm_set_wrap(slice_num1, WRAP);
    pwm_set_enabled(slice_num1, true);
    pwm_set_gpio_level(SERVO, 0);
}

uint16_t deg_to_pwm(float angle)
{
    float angle_prop = (angle / 180.0);
    float pwm_range = ((float)DUTY_MAX - (float)DUTY_MIN);
    uint16_t pwm_val = (angle_prop * pwm_range + DUTY_MIN) / 100 * WRAP;
    printf("angle prop: %.2f, pwm_range: %.0f, angle: %.2f, pwm: %u\n", angle_prop, pwm_range, angle, pwm_val);
    return pwm_val;
}

uint16_t pwm_sweep()
{
    static uint16_t step = DUTY_MIN - 1;

    step++;
    if (step > DUTY_MAX)
    {
        step = DUTY_MIN;
    }
    printf("step: %u\n", step);
    return step;
}

int main()
{
    stdio_init_all();
    int rc = init_led();
    init_pwm();
    await_usb();

    printf("Starting sweep\n");
    while (true)
    {
        pwm_set_gpio_level(SERVO, pwm_sweep());
        sleep_ms(200);
        toggle_led(0);
    }
}
