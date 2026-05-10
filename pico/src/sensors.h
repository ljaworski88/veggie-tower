#ifndef SENSORS_H
#define SENSORS_H

struct pico_sensor_data {
    double temperature;
    double humidity;
    double pressure;
    double co2;
};

int sensors_init(void);
int sensors_update(void);
const struct pico_sensor_data *sensors_get(void);

#endif /* SENSORS_H */
