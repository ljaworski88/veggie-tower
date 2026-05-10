#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "wifi.h"
#include "mqtt.h"
#include "i2c.h"
#include "sensors.h"
#include "settings.h"
#include "watering.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    LOG_INF("Veggie Tower starting up");

    /* Load persisted setpoints from flash */
    settings_load();

    /* Initialize WiFi and wait for connection */
    wifi_init();

    /* Initialize MQTT and connect to broker */
    mqtt_init();

    /* Initialize I2C bus for zone controller communication */
    i2c_init();

    /* Initialize onboard sensors */
    sensors_init();

    /* Publish current setpoints to Home Assistant to resync state */
    mqtt_publish_setpoints();

    /* Main loop */
    while (1) {
        sensors_update();
        i2c_poll_zones();
        watering_update();
        mqtt_process();
        k_sleep(K_SECONDS(1));
    }

    return 0;
}
