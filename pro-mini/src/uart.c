#include <avr/io.h>
#include "uart.h"
#include "config.h"

int uart_init(void)
{
    /* Set baud rate */
    uint16_t ubrr = F_CPU / 16 / BAUD_RATE - 1;
    UBRR0H = (ubrr >> 8);
    UBRR0L = ubrr;

    /* Enable TX and RX */
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    /* 8 data bits, 1 stop bit */
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

    return 0;
}

int uart_read_co2(uint16_t *co2_ppm)
{
    /* TODO: send MH-Z19 read command (0xFF 0x01 0x86 ...) */
    /* TODO: receive 9-byte response and parse CO2 value */
    /* TODO: verify checksum */
    *co2_ppm = 0;
    return 0;
}
