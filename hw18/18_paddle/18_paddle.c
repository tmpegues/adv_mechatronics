#include <stdio.h>
#include "pico/stdlib.h"
#include "../../pegues_lib.h"
#include "ina219.h"

#define IN1 16
#define IN2 17

#define PWMDIV 7.5
#define WRAP 1000

void init_hbridge()
{
    gpio_set_function(IN1, GPIO_FUNC_PWM);
    gpio_set_function(IN2, GPIO_FUNC_PWM);
    uint slice_num1 = pwm_gpio_to_slice_num(IN1); // With my setup, they should be the same slice, but I'll act like they're not anyway
    uint slice_num2 = pwm_gpio_to_slice_num(IN2);
    pwm_set_clkdiv(slice_num1, PWMDIV);
    pwm_set_clkdiv(slice_num2, PWMDIV);
    pwm_set_wrap(slice_num1, WRAP);
    pwm_set_wrap(slice_num2, WRAP);
    pwm_set_enabled(slice_num1, true);
    pwm_set_enabled(slice_num2, true);
    pwm_set_gpio_level(IN1, WRAP);
    pwm_set_gpio_level(IN2, WRAP);
}

void set_bridge(int duty)
{
    // printf("%d\n", duty);
    if (duty > 0)
    {
        // printf("Positive");
        pwm_set_gpio_level(IN1, WRAP);
        pwm_set_gpio_level(IN2, WRAP - duty);
    }
    else if (duty < 0)
    {
        // printf("Negative");
        pwm_set_gpio_level(IN1, WRAP + duty);
        pwm_set_gpio_level(IN2, WRAP);
    }
    else
    {
        pwm_set_gpio_level(IN1, WRAP);
        pwm_set_gpio_level(IN2, WRAP);
    }
}

int main()
{
    await_usb(0, true);
    breathe_led(0);
    init_ina219();
    init_hbridge();
    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
