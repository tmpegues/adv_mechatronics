// function prototypes for the AS5600 magnetic encoder

#ifndef AS5600_H
#define AS5600_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"

void init_as5600();
uint16_t read_as5600();

#endif
