#include "sensors.h"
#include "adc.h"
#include "bme280.h"
#include "uart.h"
#include <avr/io.h>

static struct bme280_data sensor_data;

int sensors_init(void) {
    /* Initialize BME280 via SPI */
    bme280_init();

    return 0;
}

int sensors_update(void) { return 0; }

const struct zone_sensor_data *sensors_get(void) {}
