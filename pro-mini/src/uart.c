#include "uart.h"
#include "bme280.h"
#include "config.h"
#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>

int8_t uart_init(void) {
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

int8_t uart_read_co2(uint16_t *co2_ppm) {
    /* TODO: send MH-Z19 read command (0xFF 0x01 0x86 ...) */
    /* TODO: receive 9-byte response and parse CO2 value */
    /* TODO: verify checksum */
    *co2_ppm = 0;
    return 0;
}

int8_t uart_send_byte(char data) {
    /* Make sure transfer buffer is empty */
    while (!(UCSR0A & (1 << UDRE0)))
        ;

    /* Place the data in the transmitt buffer, hardware will handle the rest */
    UDR0 = data;
    return 0;
}

int8_t send_adc_result(uint16_t adc_reading) {
    char send_buffer[6]; // max value is 1023 so we only need 4 digits
    char adc_text[] = "moisture: ";
    for (uint8_t i = 0; adc_text[i] != '\0'; i++) {
        uart_send_byte(adc_text[i]);
    }
    itoa(adc_reading, send_buffer, 10);
    for (uint8_t i = 0; send_buffer[i] != '\0'; i++) {
        uart_send_byte(send_buffer[i]);
    }
    uart_send_byte('\r');
    uart_send_byte('\n');
    return 0;
}

int8_t send_htp_data(struct bme280_data *data) {
    char send_buffer[11];
    char temp[] = "temp: ";
    char press[] = "press: ";
    char hum[] = "hum: ";

    for (uint8_t i = 0; temp[i] != '\0'; i++) {
        uart_send_byte(temp[i]);
    }
    ltoa(data->temperature, send_buffer, 10);
    for (uint8_t i = 0; send_buffer[i] != '\0'; i++) {
        uart_send_byte(send_buffer[i]);
    }
    uart_send_byte('\r');
    uart_send_byte('\n');

    for (uint8_t i = 0; press[i] != '\0'; i++) {
        uart_send_byte(press[i]);
    }
    ltoa(data->pressure, send_buffer, 10);
    for (uint8_t i = 0; send_buffer[i] != '\0'; i++) {
        uart_send_byte(send_buffer[i]);
    }
    uart_send_byte('\r');
    uart_send_byte('\n');

    for (uint8_t i = 0; hum[i] != '\0'; i++) {
        uart_send_byte(hum[i]);
    }
    ltoa(data->humidity / 1024, send_buffer, 10);
    for (uint8_t i = 0; send_buffer[i] != '\0'; i++) {
        uart_send_byte(send_buffer[i]);
    }
    uart_send_byte('\r');
    uart_send_byte('\n');
    return 0;
}
