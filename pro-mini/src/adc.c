#include <avr/io.h>
#include "adc.h"


int adc_init(void){
    /* Configure ADC for soil moisture sensors */
    ADMUX  = (1 << REFS0);                               /* AVcc reference */
    ADCSRA = (1 << ADEN)                                 /* enable ADC */
           | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); /* prescaler 128 */
    return 0;
}

uint16_t adc_read(uint8_t channel){
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADC;
}
