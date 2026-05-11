#include <stdio.h>
#include "pico/stdlib.h"
#include "../../pegues_lib/pegues_lib.h"

#define BUTTON_1 22
#define LED_1 21

volatile bool button_state = false;

void gpio_callback(uint gpio, uint32_t events)
{
    // static absolute_time_t last_time = 0;

    // if (get_absolute_time() - last_time >= 100000)
    // {
    //     button_state = !button_state;
    //     last_time = get_absolute_time();
    // }

    if (events & GPIO_IRQ_EDGE_RISE) // Sometimes both events come through, I'd rather have it fail and say rise (return to neutral) than fail and say fall
    {
        button_state = false;
    }
    else if (events & GPIO_IRQ_EDGE_FALL)
    {
        button_state = true;
    }
}

int main()
{
    await_usb(0, true);

    gpio_init(BUTTON_1);
    gpio_set_dir(BUTTON_1, GPIO_IN);

    gpio_init(LED_1);
    gpio_set_dir(LED_1, GPIO_OUT);
    gpio_put(LED_1, 1);

    gpio_set_irq_enabled_with_callback(BUTTON_1, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true,
                                       &gpio_callback);

    while (true)
    {
        if (button_state)
        {
            gpio_put(LED_1, 1);
        }
        else
        {
            gpio_put(LED_1, 0);
        }
        printf("b: %d\n", button_state);
    }
}
