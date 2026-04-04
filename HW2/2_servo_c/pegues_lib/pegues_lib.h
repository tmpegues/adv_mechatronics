#ifndef PEGUES_H
#define PEGUES_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"

#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

void toggle_led(int state); // state -1 or 1 sets LED to off or on. state 0 toggles from whatever it currently is.
int init_led(); // init built in LED
void await_usb(); // Waits for USB serial connection.

#endif