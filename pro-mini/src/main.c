#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "config.h"
#include "i2c_peripheral.h"
#include "uart.h"
#include "sensors.h"
#include "actuators.h"
#include "adc.h"

int main(void) {
    uint16_t adc_result;
    /* Initialize all subsystems */
    uart_init();
    adc_init();
    //sensors_init();
    //actuators_init();
    //i2c_peripheral_init();

    /* Enable global interrupts (required for I2C peripheral) */
    sei();

    while (1) {
        /* Update sensor readings */
        //sensors_update();

        adc_result = adc_read(0);
        send_adc_result(adc_result);

        /* I2C communication is interrupt driven —
         * main loop just keeps sensor data fresh
         * for the ISR to respond with */
        _delay_ms(500);
    }

    return 0;
}
