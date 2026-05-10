#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "watering.h"
#include "settings.h"
#include "i2c.h"
#include "mqtt.h"

LOG_MODULE_REGISTER(watering, LOG_LEVEL_INF);

int watering_update(void)
{
    /*
     * TODO: per zone, per subzone:
     * 1. Check averaged soil moisture
     * 2. If below moisture_low, begin watering cycle:
     *    a. Command valve open for watering_duration seconds via I2C
     *    b. Wait watering_settle_time seconds
     *    c. Re-check moisture
     *    d. If below moisture_target, recalculate duration and repeat
     *    e. Repeat up to watering_max_cycles, then alert HA if target not reached
     */
    return 0;
}
