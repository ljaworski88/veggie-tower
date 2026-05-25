#ifndef ACTUATORS_H
#define ACTUATORS_H

#include <stdint.h>

uint8_t actuators_init(void);
uint8_t actuators_set_valve(uint8_t valve, uint8_t state);
uint8_t actuators_set_fan(uint8_t duty_cycle);
uint8_t actuators_set_lights(uint8_t duty_cycle);

#endif /* ACTUATORS_H */
