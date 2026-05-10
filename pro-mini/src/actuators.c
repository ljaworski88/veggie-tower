#include <avr/io.h>
#include "actuators.h"
#include "config.h"

int actuators_init(void)
{
    /* Configure valve pins as outputs */
    DDRD |= (1 << VALVE_1_PIN) | (1 << VALVE_2_PIN) | (1 << VALVE_3_PIN);
    DDRB |= (1 << VALVE_4_PIN);

    /* Fan PWM — Timer2, Phase correct PWM on OC2B (D3) */
    DDRD  |= (1 << FAN_PWM_PIN);
    TCCR2A = (1 << COM2B1) | (1 << WGM20);  /* OC2B only, phase correct */
    TCCR2B = (1 << CS21);                    /* prescaler 8 */
    OCR2B  = 0;                              /* start at 0% */

    /* Lights PWM — Timer1, Phase correct PWM on OC1A (D9) */
    DDRB  |= (1 << LIGHTS_PWM_PIN);
    TCCR1A = (1 << COM1A1) | (1 << WGM10);  /* OC1A only, phase correct 8-bit */
    TCCR1B = (1 << CS11);                    /* prescaler 8 */
    OCR1A  = 0;                              /* start off */

    return 0;
}

int actuators_set_valve(uint8_t valve, uint8_t state)
{
    /* TODO: set the appropriate valve pin high/low */
    return 0;
}

int actuators_set_fan(uint8_t ocr_value)
{
    OCR2B = ocr_value;
    return 0;
}

int actuators_set_lights(uint8_t ocr_value)
{
    OCR1A = ocr_value;
    return 0;
}
