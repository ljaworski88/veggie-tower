#ifndef ACTUATORS_H
#define ACTUATORS_H

#include <stdint.h>

int actuators_init(void);
int actuators_set_valve(uint8_t valve, uint8_t state);
int actuators_set_fan(uint8_t duty_cycle);
int actuators_set_lights(uint8_t duty_cycle);

#endif /* ACTUATORS_H */
