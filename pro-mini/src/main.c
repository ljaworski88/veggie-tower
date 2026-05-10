#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "config.h"
#include "i2c_peripheral.h"
#include "uart.h"
#include "sensors.h"
#include "actuators.h"

int main(void)
{
    /* Initialize all subsystems */
    uart_init();
    sensors_init();
    actuators_init();
    i2c_peripheral_init();

    /* Enable global interrupts (required for I2C peripheral) */
    sei();

    while (1) {
        /* Update sensor readings */
        sensors_update();

        /* I2C communication is interrupt driven —
         * main loop just keeps sensor data fresh
         * for the ISR to respond with */
        _delay_ms(500);
    }

    return 0;
}
