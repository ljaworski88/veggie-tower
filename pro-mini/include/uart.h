#ifndef UART_H
#define UART_H

#include <stdint.h>

int uart_init(void);
int uart_read_co2(uint16_t *co2_ppm);
int uart_send_byte(char data);
int send_adc_result(uint16_t adc_reading);

#endif /* UART_H */
