#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/uart.h"
#include "../../pegues_lib/pegues_lib.h"

// UART defines
// By default the stdout UART is `uart0`, so we will use the second one
#define UART_ID uart0
#define BAUD_RATE 115200

// Use pins 4 and 5 for UART1
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define UART_TX_PIN 0
#define UART_RX_PIN 1

int main()
{
    await_usb(0, true);

    // Set up our UART
    uart_init(UART_ID, BAUD_RATE);
    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Use some the various UART functions to send out data
    // In a default system, printf will also output via the default UART

    // Send out a string, with CR/LF conversions
    uart_puts(UART_ID, " Hello, UART!\n");

    // For more examples of UART use see https://github.com/raspberrypi/pico-examples/tree/master/uart

    int txNumber;
    char txMessage[100];

    int rxNumber;
    char rxMessage[100];

    while (true)
    {
        breathe_led(0);
        while (uart_is_readable(UART_ID))
        {
            uart_read_blocking(UART_ID, rxMessage, 100);
            scanf(rxMessage, "%d", &rxNumber);
            printf("%d\n", rxNumber);
        }
        while (uart_is_writable(UART_ID))
        {
            scanf("%d", &txNumber);
            sprintf(txMessage, "%d", txNumber);
            printf("received %d\n", txMessage);
            uart_write_blocking(UART_ID, txMessage, strlen(txMessage));
        }
    }
}
