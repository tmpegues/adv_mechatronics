#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "../../pegues_lib/pegues_lib.h"

// I2C defines
// This example will use I2C0 on GPIO4 (SDA) and GPIO5 (SCL) running at 100KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c_default
#define I2C_SDA 4 // Pin 6
#define I2C_SCL 5 // Pin 7

#define ADDR 0b0100000
#define IODIR_ADDR _u(0x00)
#define GPIO_ADDR _u(0x09)
#define OLAT_ADDR _u(0x0A)

void init_mcp23008()
{
    stdio_init_all();

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);

    // I2C Initialisation. Using it at 100Khz.
    i2c_init(I2C_PORT, 100 * 1000);

    // initialize the MCP23008 with GP7 connected to an LED (write/output/0) and GP0 connected to a button (read/input/1)
    uint8_t buf[2];
    buf[0] = IODIR_ADDR;
    buf[1] = 0u << 7 | 1u << 0;

    i2c_write_blocking(I2C_PORT, ADDR, buf, 2, false);
}

// General I2C functions that we were recommended to write
void setPin(uint8_t address, uint8_t reg, uint8_t value)
{
    uint8_t buf[2] = {reg, value};
    i2c_write_blocking(I2C_PORT, address, buf, 2, false);
}

uint8_t readPin(uint8_t address, uint8_t reg)
{
    uint8_t buf;
    i2c_write_blocking(i2c_default, ADDR, &reg, 1, true); // true to keep host control of bus
    i2c_read_blocking(i2c_default, ADDR, &buf, 1, false); // false - finished with bus
    // printf("All pins: %d\n", buf);
    return buf;
}


// Specific MCP23008 functions using the above
void write_mcp23008_gpio(int pin, uint state)
{
    setPin(ADDR, GPIO_ADDR, state << pin);
}

// Assumes pull up resistor
bool read_mcp23008_gpio(int pin)
{
    bool pin_state = !(readPin(ADDR, GPIO_ADDR) << pin & 1u);
    // printf("Pin, state %d: %d\n", pin, pin_state);
    return pin_state;
}

int main()
{
    await_usb(0);
    init_mcp23008();

    while (true)
    {
        toggle_led(0);
        write_mcp23008_gpio(7, read_mcp23008_gpio(0));
        sleep_ms(25); // Using 25 so it's slow enough that you can see the built in LED blink
    }
}