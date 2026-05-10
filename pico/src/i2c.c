#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "i2c.h"

LOG_MODULE_REGISTER(i2c, LOG_LEVEL_INF);

int i2c_init(void)
{
    /* TODO: initialize I2C bus */
    return 0;
}

int i2c_poll_zones(void)
{
    /* TODO: poll each zone controller for sensor data */
    /* TODO: forward received data to mqtt telemetry */
    return 0;
}
