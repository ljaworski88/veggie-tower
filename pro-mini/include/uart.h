#ifndef UART_H
#define UART_H

#include <stdint.h>

int uart_init(void);
int uart_read_co2(uint16_t *co2_ppm);

#endif /* UART_H */
