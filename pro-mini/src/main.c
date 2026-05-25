#include "actuators.h"
#include "adc.h"
#include "config.h"
#include "i2c_peripheral.h"
#include "sensors.h"
#include "uart.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

int main(void) {
    uint16_t adc_result;
    struct bme280_data sensor_data;
    uint8_t return_code;
    char error_code[5];
    /* Initialize all subsystems */
    uart_init();
    adc_init();
    return_code = bme280_init();
    itoa(return_code, error_code, 10);

    for (uint8_t i = 0; error_code[i] != '\0'; i++) {
        uart_send_byte(error_code[i]);
    }
    uart_send_byte('e');
    uart_send_byte('\r');
    uart_send_byte('\n');
    // sensors_init();
    // actuators_init();
    // i2c_peripheral_init();

    /* Enable global interrupts (required for I2C peripheral) */
    sei();

    while (1) {
        /* Update sensor readings */
        // sensors_update();

        adc_result = adc_read(0);
        bme280_read(&sensor_data);
        send_adc_result(adc_result);
        send_htp_data(&sensor_data);

        /* I2C communication is interrupt driven —
         * main loop just keeps sensor data fresh
         * for the ISR to respond with */
        _delay_ms(500);
    }

    return 0;
}
