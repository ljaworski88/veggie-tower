#ifndef I2C_PERIPHERAL_H
#define I2C_PERIPHERAL_H

#include <stdint.h>

/* Command bytes sent from Pico W */
#define CMD_GET_SENSORS 0x01
#define CMD_SET_VALVE 0x02
#define CMD_SET_FAN 0x03
#define CMD_SET_LIGHTS 0x04

uint8_t i2c_peripheral_init(void);

#endif /* I2C_PERIPHERAL_H */
