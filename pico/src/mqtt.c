#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "mqtt.h"

LOG_MODULE_REGISTER(mqtt, LOG_LEVEL_INF);

int mqtt_init(void)
{
    /* TODO: connect to broker using credentials from prj.conf */
    /* TODO: subscribe to setpoint topics */
    return 0;
}

int mqtt_process(void)
{
    /* TODO: handle incoming setpoint messages */
    /* TODO: publish queued telemetry */
    return 0;
}

int mqtt_publish_setpoints(void)
{
    /* TODO: publish current setpoints for all zones to resync HA state */
    return 0;
}

int mqtt_publish_telemetry(const char *topic, const char *payload)
{
    /* TODO: publish a telemetry message */
    return 0;
}
