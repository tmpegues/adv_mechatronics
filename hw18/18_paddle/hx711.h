// function prototypes for the HX711 load cell amplifier

#ifndef HX711_H
#define HX711_H

#include "pico/stdlib.h"

void init_hx711();
int32_t read_hx711();

#endif
