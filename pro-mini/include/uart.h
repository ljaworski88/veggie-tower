#ifndef UART_H
#define UART_H

#include "bme280.h"
#include <stdint.h>

int8_t uart_init(void);
int8_t uart_read_co2(uint16_t *co2_ppm);
int8_t uart_send_byte(char data);
int8_t send_adc_result(uint16_t adc_reading);
int8_t send_htp_data(struct bme280_data *data);

#endif /* UART_H */
