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

void await_usb(float await_seconds)
{
    stdio_init_all();
    int rc = init_led();
    int await_sleep = 50;
    bool indefinite = false;
    if (await_seconds == 0)
    {
        indefinite = true;
    }
    int max_loops = await_seconds * 1000.0 / await_sleep;
    int await_loop = 0;
    while (!stdio_usb_connected() && (indefinite || await_loop <= max_loops + 1))
    {
        toggle_led(0);
        sleep_ms(await_sleep); // Blink fast while waiting
        await_loop++;
    }
    toggle_led(-1);

    // SOS pattern to indicate connected
    int blink_delay = 25;
    for (int i = 0; i <= 3; i++)
    {
        toggle_led(1);
        sleep_ms(blink_delay);
        toggle_led(-1);
        sleep_ms(blink_delay);
    }
    sleep_ms(blink_delay * 3);
    for (int i = 0; i <= 3; i++)
    {
        toggle_led(1);
        sleep_ms(blink_delay * 3);
        toggle_led(-1);
        sleep_ms(blink_delay);
    }
    sleep_ms(blink_delay * 3);
    for (int i = 0; i <= 3; i++)
    {
        toggle_led(1);
        sleep_ms(blink_delay);
        toggle_led(-1);
        sleep_ms(blink_delay);
    }
    printf("USB serial connected after %d loops\n", await_loop);
}