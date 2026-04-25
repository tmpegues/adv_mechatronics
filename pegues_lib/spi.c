#include "./spi.h"

#define SPI_PORT spi1
#define PIN_MISO 12
#define PIN_SCK 10
#define PIN_MOSI 11

void init_spi()
{
    spi_init(SPI_PORT, 1000 * 1000);

    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
}

void init_spi_cs(int cs_pin)
{ // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_set_function(cs_pin, GPIO_FUNC_SIO);
    gpio_set_dir(cs_pin, GPIO_OUT);
    gpio_put(cs_pin, 1);
}

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

void write_dac(int cs_pin, uint16_t a_value, uint16_t b_value)
{
    cs_select(cs_pin);
    uint8_t data[8];
    if (a_value <= 0b1111111111) // We've only got 10 bits available

    {
        data[0] = 0b0111 << 4 | ((a_value >> 6) & 0b00001111);
        data[1] = a_value << 2 & 0b11111100;

        spi_write_blocking(SPI_PORT, data, 2); // where data is a uint8_t array with length len
    }
    if (b_value <= 0b1111111111)
    {
        uint8_t data[8];
        data[0] = 0b1111 << 4 | ((b_value >> 6) & 0b00001111);
        data[1] = b_value << 2 & 0b11111100;

        spi_write_blocking(SPI_PORT, data, 2);
    }
    cs_deselect(cs_pin);
}