#ifndef PEGUES_H
#define PEGUES_H

#include <stdio.h>
#include "pico/stdlib.h"

#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif


// init built in LED
int init_led();

// state -1 or 1 sets LED to off or on. state 0 toggles current state.
void toggle_led(int state);

// Waits for USB serial connection.
// If await_seconds = 0, it will wait indefinitely
void await_usb(float await_seconds);

#endif