#include <avr/io.h>
#include "actuators.h"
#include "config.h"

int actuators_init(void)
{
    /* Configure valve pins as outputs */
    DDRD |= (1 << VALVE_1_PIN) | (1 << VALVE_2_PIN) | (1 << VALVE_3_PIN);
    DDRB |= (1 << VALVE_4_PIN);

    /* Configure fan and light PWM - Timer2, Phase correct PWM */
    DDRD  |= (1 << FAN_PWM_PIN);
    DDRB  |= (1 << LIGHTS_PWM_PIN);
    TCCR2A = (1 << COM2A1) | (1 << COM2B1) | (1 << WGM20);
    TCCR2B = (1 << CS21);   /* prescaler 8 */
    OCR2A  = 0;              /* start at 0% */
    OCR2B  = 0;              /* start at 0% */

    return 0;
}

int actuators_set_valve(uint8_t valve, uint8_t state)
{
    /* TODO: set the appropriate valve pin high/low */
    return 0;
}

int actuators_set_fan(uint8_t duty_cycle)
{
    OCR2B = duty_cycle;
    return 0;
}

int actuators_set_lights(uint8_t duty_cycle)
{
    OCR2A = duty_cycle;
    return 0;
}
