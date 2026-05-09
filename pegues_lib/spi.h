#ifndef PEGUES_SPI
#define PEGUES_SPI

#include "hardware/spi.h"
#include "pico/stdlib.h"
#include <stdio.h>

// init_spi does not set any chip select pins
void init_spi();

void init_spi_cs(int cs_pin);

static inline void cs_select(uint cs_pin);

static inline void cs_deselect(uint cs_pin);

void write_dac(int cs_pin, uint16_t a_value, uint16_t b_value);

void init_ram(int);

#endif