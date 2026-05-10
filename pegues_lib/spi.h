#ifndef PEGUES_SPI
#define PEGUES_SPI

#include "hardware/spi.h"
#include "pico/stdlib.h"
#include <stdio.h>

static inline void cs_select(uint cs_pin)
{
    asm volatile("nop \n nop \n nop"); // FIXME
    gpio_put(cs_pin, 0);
    asm volatile("nop \n nop \n nop"); // FIXME
}

static inline void cs_deselect(uint cs_pin)
{
    asm volatile("nop \n nop \n nop"); // FIXME
    gpio_put(cs_pin, 1);
    asm volatile("nop \n nop \n nop"); // FIXME
}

// init_spi does not set any chip select pins
void init_spi();

void init_spi_cs(int cs_pin);

void write_dac(int cs_pin, uint16_t a_value, uint16_t b_value);

void init_ram(int);

void ram_write_one(uint8_t cs_pin, uint8_t addr, uint8_t value);

#endif