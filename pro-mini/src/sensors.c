#include <avr/io.h>
#include "sensors.h"
#include "uart.h"
#include "bme280.h"

static struct zone_sensor_data sensor_data;

int sensors_init(void)
{
    /* Configure ADC for soil moisture sensors */
    ADMUX  = (1 << REFS0);                               /* AVcc reference */
    ADCSRA = (1 << ADEN)                                 /* enable ADC */
           | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); /* prescaler 128 */

    /* Initialize BME280 via SPI */
    bme280_init();

    return 0;
}

static uint16_t adc_read(uint8_t channel)
{
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADC;
}

int sensors_update(void)
{
    /* Read soil moisture sensors */
    for (uint8_t i = 0; i < 4; i++) {
        sensor_data.soil_moisture[i] = adc_read(i);
    }

    /* Read CO2 from MH-Z19 via UART */
    uart_read_co2(&sensor_data.co2_ppm);

    /* Read temperature, humidity, and pressure from BME280 via SPI */
    bme280_read(&sensor_data.temperature_c,
                &sensor_data.humidity_pct,
                &sensor_data.pressure_pa);

    return 0;
}

const struct zone_sensor_data *sensors_get(void)
{
    return &sensor_data;
}
