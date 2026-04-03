#ifndef PEGUES_H
#define PEGUES_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"

#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

void toggle_led(int state);
int init_led();
void await_usb();

#endif