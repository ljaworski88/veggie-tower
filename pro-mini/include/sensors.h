#ifndef SENSORS_H
#define SENSORS_H

#include <stdint.h>

struct zone_sensor_data {
    uint16_t soil_moisture[4];  /* raw ADC readings, one per sensor */
    uint16_t co2_ppm;
    int16_t  temperature_c;     /* scaled x10, e.g. 215 = 21.5C */
    uint16_t humidity_pct;      /* scaled x10, e.g. 654 = 65.4% */
};

int sensors_init(void);
int sensors_update(void);
const struct zone_sensor_data *sensors_get(void);

#endif /* SENSORS_H */
