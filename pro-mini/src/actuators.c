#include "actuators.h"
#include "config.h"
#include <avr/io.h>

uint8_t actuators_init(void) {
    // Configure valve pins as outputs
    DDRD |= (1 << VALVE_1) | (1 << VALVE_2) | (1 << VALVE_3);
    DDRB |= (1 << VALVE_4);

    // Fan PWM — Timer2, Phase correct PWM on OC2B (D3)
    DDRD |= (1 << FAN_PWM);
    TCCR2A = (1 << COM2B1) | (1 << WGM20); // OC2B only, phase correct
    TCCR2B = (1 << CS21);                  // prescaler 8
    OCR2B = 0;                             // start off

    // Lights PWM — Timer1, Phase correct PWM on OC1A (D9)
    DDRB |= (1 << LIGHTS_PWM);
    TCCR1A = (1 << COM1A1) | (1 << WGM10); // OC1A only, phase correct 8-bit
    TCCR1B = (1 << CS11);                  // prescaler 8
    OCR1A = 0;                             // start off

    return 0;
}

uint8_t actuators_set_valve(uint8_t valve, uint8_t state) {
    // TODO: set the appropriate valve pin high/low
    return 0;
}

uint8_t actuators_set_fan(uint8_t pwm) {
    OCR2B = pwm;
    return 0;
}

uint8_t actuators_set_lights(uint8_t pwm) {
    OCR1A = pwm;
    return 0;
}
