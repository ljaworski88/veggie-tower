#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "sensors.h"

LOG_MODULE_REGISTER(sensors, LOG_LEVEL_INF);

static struct pico_sensor_data sensor_data;

int sensors_init(void)
{
    /* TODO: initialize onboard temp/humidity/pressure/CO2 sensors */
    return 0;
}

int sensors_update(void)
{
    /* TODO: read and update sensor_data */
    return 0;
}

const struct pico_sensor_data *sensors_get(void)
{
    return &sensor_data;
}
