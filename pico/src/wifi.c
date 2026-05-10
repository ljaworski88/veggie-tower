#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "wifi.h"

LOG_MODULE_REGISTER(wifi, LOG_LEVEL_INF);

int wifi_init(void)
{
    /* TODO: implement WiFi connection using credentials from prj.conf */
    return 0;
}

bool wifi_is_connected(void)
{
    /* TODO: return actual connection status */
    return false;
}
