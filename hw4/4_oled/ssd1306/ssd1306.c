// based on adafruit and sparkfun libraries

#include <string.h> // for memset
#include <stdio.h>

#include "ssd1306.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include "font.h"

unsigned char SSD1306_ADDRESS = 0b0111100; // 7bit i2c address
unsigned char ssd1306_buffer[513];         // 128x32/8. Every bit is a pixel except first byte

void ssd1306_setup()
{

    // first byte in ssd1306_buffer is a command
    ssd1306_buffer[0] = 0x40;
    // give a little delay for the ssd1306 to power up
    sleep_ms(20);
    ssd1306_command(SSD1306_DISPLAYOFF);
    ssd1306_command(SSD1306_SETDISPLAYCLOCKDIV);
    ssd1306_command(0x80);
    ssd1306_command(SSD1306_SETMULTIPLEX);
    ssd1306_command(0x1F); // height-1 = 31
    ssd1306_command(SSD1306_SETDISPLAYOFFSET);
    ssd1306_command(0x0);
    ssd1306_command(SSD1306_SETSTARTLINE);
    ssd1306_command(SSD1306_CHARGEPUMP);
    ssd1306_command(0x14);
    ssd1306_command(SSD1306_MEMORYMODE);
    ssd1306_command(0x00);
    ssd1306_command(SSD1306_SEGREMAP | 0x1);
    ssd1306_command(SSD1306_COMSCANDEC);
    ssd1306_command(SSD1306_SETCOMPINS);
    ssd1306_command(0x02);
    ssd1306_command(SSD1306_SETCONTRAST);
    ssd1306_command(0x8F);
    ssd1306_command(SSD1306_SETPRECHARGE);
    ssd1306_command(0xF1);
    ssd1306_command(SSD1306_SETVCOMDETECT);
    ssd1306_command(0x40);
    ssd1306_command(SSD1306_DISPLAYON);
    ssd1306_clear();
    ssd1306_update();
}

// send a command instruction (not pixel data)
void ssd1306_command(unsigned char c)
{

    uint8_t buf[2];
    buf[0] = 0x00;
    buf[1] = c;
    i2c_write_blocking(i2c_default, SSD1306_ADDRESS, buf, 2, false);
}

// update every pixel on the screen
void ssd1306_update()
{
    ssd1306_command(SSD1306_PAGEADDR);
    ssd1306_command(0);
    ssd1306_command(0xFF);
    ssd1306_command(SSD1306_COLUMNADDR);
    ssd1306_command(0);
    ssd1306_command(128 - 1); // Width

    unsigned short count = 512;          // WIDTH * ((HEIGHT + 7) / 8)
    unsigned char *ptr = ssd1306_buffer; // first address of the pixel buffer

    i2c_write_blocking(i2c_default, SSD1306_ADDRESS, ptr, 513, false);
}

// set a pixel value. Call update() to push to the display)
void ssd1306_drawPixel(unsigned char x, unsigned char y, unsigned char color)
{
    if ((x < 0) || (x >= 128) || (y < 0) || (y >= 32))
    {
        return;
    }

    if (color == 1)
    {
        ssd1306_buffer[1 + x + (y / 8) * 128] |= (1 << (y & 7));
    }
    else
    {
        ssd1306_buffer[1 + x + (y / 8) * 128] &= ~(1 << (y & 7));
    }

}

void ssd1306_drawLetter(unsigned char x_or, unsigned char y_or, unsigned char character)
{
    // Using a basic loop for now to make sure I understand the structure correctly
    printf("ASCII: %d\n", character);
    character -= 0x20;
    printf("ASCII: %d\n", character);

    for (int x = 0; x < 5; x++)
    {
        char col = ASCII[character][x];
        printf("col: %x\n", col);
        for (int y = 0; y < 8; y++)
        {
            bool pin_on = ((col >> y) & 1);
            ssd1306_drawPixel(x_or + x, y_or - y, pin_on);
            printf("x, y, on: %d, %d, %d\n", x_or + x, y_or - y, pin_on);
        }
    }

}

// zero every pixel value
void ssd1306_clear()
{
    memset(ssd1306_buffer, 0, 512); // make every bit a 0, memset in string.h
    ssd1306_buffer[0] = 0x40;       // first byte is part of command
}

// 01111111  H
// 00001000
// 00001000
// 00001000
// 01111111

// 00110010  @
// 01001001
// 01111001
// 01000001
// 00111110

// 00100000  J
// 01000000
// 01000001
// 00111111
// 00000001