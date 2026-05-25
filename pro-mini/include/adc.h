#ifndef ADC_H
#define ADC_H

#include <stdint.h>

uint8_t adc_init(void);
uint16_t adc_read(uint8_t channel);

#endif
