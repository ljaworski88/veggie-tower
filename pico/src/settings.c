#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/settings/settings.h>
#include "settings.h"

LOG_MODULE_REGISTER(settings, LOG_LEVEL_INF);

static struct zone_setpoints zone_setpoints[ZONE_COUNT];

int settings_load(void)
{
    /* TODO: load setpoints from NVS flash */
    return 0;
}

int settings_save(void)
{
    /* TODO: persist setpoints to NVS flash */
    return 0;
}

const struct zone_setpoints *settings_get_zone(uint8_t zone)
{
    return &zone_setpoints[zone];
}

int settings_set_zone(uint8_t zone, const struct zone_setpoints *setpoints)
{
    zone_setpoints[zone] = *setpoints;
    return settings_save();
}
