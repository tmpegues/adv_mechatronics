#include "pegues_lib.h"

#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

// Turn the led on or off, I use it to know that the loop is running properly

void toggle_led(int state)
{
    static bool led_on = true;
    if (state == 1)
    {
        led_on = true;
    }
    else if (state == -1)
    {
        led_on = false;
    }
#if defined(PICO_DEFAULT_LED_PIN)
    // Just set the GPIO on or off
    gpio_put(PICO_DEFAULT_LED_PIN, led_on);
#elif defined(CYW43_WL_GPIO_LED_PIN)
    // Ask the wifi "driver" to set the GPIO on or off
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);
#endif
    led_on = !led_on;
}

int init_led()
{
#if defined(PICO_DEFAULT_LED_PIN)
    // A device like Pico that uses a GPIO for the LED will define PICO_DEFAULT_LED_PIN
    // so we can use normal GPIO functionality to turn the led on and off
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    return PICO_OK;

#elif defined(CYW43_WL_GPIO_LED_PIN)
    // For Pico W devices we need to initialise the driver etc
    int driver_init = cyw43_arch_init();
    toggle_led(1);
    return driver_init;

#endif
}

void await_usb()
{
    while (!stdio_usb_connected())
    {
        printf("Seeking USB\n");
        toggle_led(0);
        sleep_ms(50); // Blink fast while waiting
    }
    // Blink pattern to indicate connected
    for (int i = 0; i <= 10; i++)
    {
        toggle_led(0);
        sleep_ms(20);
    }
    for (int i = 0; i <= 10; i++)
    {
        toggle_led(0);
        sleep_ms(100);
    }
    for (int i = 0; i <= 10; i++)
    {
        toggle_led(0);
        sleep_ms(20);
    }
    printf("USB Serial Connected\n");
}