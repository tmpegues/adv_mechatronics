// based on adafruit and sparkfun libraries

#include <string.h> // for memset
#include <stdio.h>
#include <stdlib.h>

#include "ssd1306.h"
#include "hardware/i2c.h"
#include "font.h"

#define FPS_THRESH 5
#define I2C_PORT i2c_default
#define I2C_SDA 4 // Pin 6
#define I2C_SCL 5 // Pin 7

unsigned char SSD1306_ADDRESS = 0b0111100; // 7bit i2c address of oled display
unsigned char ssd1306_buffer[513];         // 128x32/8. Every bit is a pixel except first byte
bool print_fps = true;

void ssd1306_setup(bool want_fps)
{
    // first byte in ssd1306_buffer is a command
    ssd1306_buffer[0] = 0x40;
    // give a little delay for the ssd1306 to power up

    print_fps = want_fps;
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
    printf("SSD1306 display setup complete\n");
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

    if (print_fps)
    {
        ssd1306_fps();
    }

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
    character -= 0x20;

    for (int x = 0; x <= 4; x++)
    {
        char col = ASCII[character][x];
        for (int y = 0; y <= 7; y++)
        {
            bool pin_on = ((col >> y) & 1);
            ssd1306_drawPixel(x_or + x, y_or + y, pin_on);
        }
    }
}

void ssd1306_drawString(unsigned char x_or, unsigned char y_or, unsigned char *character)
{
    int i = 0;
    int x = 0;
    int y = 0;
    while (true)
    {
        char letter = character[i];
        if (letter == 0)
        {
            break;
        }
        else
        {
            ssd1306_drawLetter(x_or + x, y_or + y, letter);
            x += 5;
            if (x_or + x > 122)
            {
                if (y_or + y > 23)
                {
                    break;
                }
                x = 0;
                y += 8;
            }
        }
        i++;
    }
}

// zero every pixel value
void ssd1306_clear()
{
    memset(ssd1306_buffer, 0, 512); // make every bit a 0, memset in string.h
    ssd1306_buffer[0] = 0x40;       // first byte is part of command
}

void ssd1306_fill()
{
    for (int x = 0; x <= 127; x++)
    {
        for (int y = -1; y <= 31; y++)
        {
            ssd1306_drawPixel(x, y, 1);
        }
    }
}

void ssd1306_fps(void)
{
    static unsigned char frame_count = 0;
    static uint last_time = 0;
    static bool colon_on = true;
    static char message[3];

    // print_fps = true;
    frame_count++;
    if (frame_count >= FPS_THRESH)
    {
        uint time = to_us_since_boot(get_absolute_time());
        uint fps = (frame_count * 1000000) / (time - last_time); // Multiply frames by 1000000 because of microseconds

        if (colon_on)
        {
            sprintf(message, "%3u", fps);
        }
        else
        {
            sprintf(message, "%3u", fps);
        }

        last_time = time;
        frame_count = 0;
        colon_on = !colon_on;
    }
    ssd1306_drawString(0, 24, message);
}

void draw_axes(int x, int y, int z)
{
    // Draw x and y lines
    // Multiply by 1/2 of screen dimension to scale 1g to screen size
    // Add 1/2 of screen dimension to center the line
    int i = 0;
    while (-64 <= i && i <= 64)
    {
        if ((x <= i && i <= 0) || (0 <= i && i <= x))
        {
            ssd1306_drawPixel(64 + i, 15, 1);
            ssd1306_drawPixel(64 + i, 16, 1);
        }
        (x >= 0) ? i++ : i--;
    }

    i = 0;
    while (-32 <= i && i <= 32)
    {
        if ((y <= i && i <= 0) || (0 <= i && i <= y))
        {
            ssd1306_drawPixel(63, 16 + i, 1);
            ssd1306_drawPixel(64, 16 + i, 1);
        }
        (y >= 0) ? i++ : i--;
    }
}

void draw_vector(int x, int y, int z)
{
    // x, y, z are the endpoints of the vector.
    for (int i = 0; i < 10; i++)
    {
        int x_px = 63 + x / 10 * i;
        int y_px = 16 + y / 10 * i;
        if (z >= 0)
        {
            ssd1306_drawPixel(x_px, y_px, 1);
            ssd1306_drawPixel(x_px + 1, y_px, 1);
            ssd1306_drawPixel(x_px - 1, y_px, 1);
            ssd1306_drawPixel(x_px, y_px + 1, 1);
            ssd1306_drawPixel(x_px, y_px - 1, 1);
        }
        else
        {
            ssd1306_drawPixel(x_px, y_px, 1);
            ssd1306_drawPixel(x_px + 1, y_px, 1);
            ssd1306_drawPixel(x_px - 1, y_px, 1);
        }
    }
}