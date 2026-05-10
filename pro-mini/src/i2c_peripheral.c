#include <avr/io.h>
#include <avr/interrupt.h>
#include "i2c_peripheral.h"
#include "config.h"
#include "sensors.h"
#include "actuators.h"

int i2c_peripheral_init(void)
{
    /* Set I2C address */
    TWAR = (ZONE_ADDR << 1);

    /* Enable TWI, ACK, and interrupt */
    TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWIE);

    return 0;
}

ISR(TWI_vect)
{
    /* TODO: handle I2C commands from Pico W:
     * CMD_GET_SENSORS  — respond with sensor data struct
     * CMD_SET_VALVE    — open/close specified valve
     * CMD_SET_FAN      — set fan PWM duty cycle
     * CMD_SET_LIGHTS   — set lights on/off
     */
    TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWINT) | (1 << TWIE);
}
