#include "./spi.h"

#define SPI_PORT spi1
#define PIN_MISO 12
#define PIN_SCK 10
#define PIN_MOSI 11

#define RAM_READ 0b00000011
#define RAM_WRITE 0b00000010

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

void write_dac(int cs_pin, uint16_t a_value, uint16_t b_value)
{
    cs_select(cs_pin);
    uint8_t data[8];
    if (a_value <= 0b1111111111) // We've only got 10 bits available

    { // MCP4912 write register (A[0]/B[1])(BUF)(GA)(SHDN)(4 bits of value), (6 bits of value)(2 bits ignored)
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

void init_ram(int cs_pin)
{
    uint8_t data[2];
    data[0] = 0b00000001; // Instruction to write to status register
    data[1] = 0b01000000; // (01 = sequential)(5 bits empty)(last bit 0 enables HOLD)
    cs_select(cs_pin);
    spi_write_blocking(SPI_PORT, data, 2);
    cs_deselect(cs_pin);
}

uint32_t ram_read_float(uint8_t cs_pin, uint16_t addr)
{
    uint8_t read_instr[3];
    read_instr[0] = 0b00000011;
    read_instr[1] = addr >> 8;
    read_instr[2] = addr & 0xFF;
    uint8_t read_data[4];

    cs_select(cs_pin);
    spi_write_blocking(SPI_PORT, read_instr, 3);
    spi_read_blocking(SPI_PORT, 0, read_data, 4);
    cs_deselect(cs_pin);

    uint32_t value = read_data[0] << 24 | read_data[1] << 16 | read_data[2] << 8 | read_data[3];
    return value;
}
