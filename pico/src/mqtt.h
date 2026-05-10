#ifndef MQTT_H
#define MQTT_H

int mqtt_init(void);
int mqtt_process(void);
int mqtt_publish_setpoints(void);
int mqtt_publish_telemetry(const char *topic, const char *payload);

#endif /* MQTT_H */
