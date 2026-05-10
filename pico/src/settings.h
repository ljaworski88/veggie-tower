#ifndef SETTINGS_H
#define SETTINGS_H

#define SUBZONE_COUNT 4

struct subzone_setpoints {
    uint32_t moisture_low;
    uint32_t moisture_target;
    uint32_t watering_duration;
    uint32_t watering_settle_time;
    uint32_t watering_max_cycles;
    uint8_t  sensor_map[SUBZONE_COUNT]; /* which analog sensor belongs to this subzone */
};

struct zone_setpoints {
    struct subzone_setpoints subzones[SUBZONE_COUNT];
    uint8_t  fan_speed;        /* 0-100% */
    uint8_t  light_on_hour;
    uint8_t  light_on_minute;
    uint8_t  light_off_hour;
    uint8_t  light_off_minute;
    uint32_t co2_threshold;
    uint32_t temp_threshold;
};

int settings_load(void);
int settings_save(void);
const struct zone_setpoints *settings_get_zone(uint8_t zone);
int settings_set_zone(uint8_t zone, const struct zone_setpoints *setpoints);

#endif /* SETTINGS_H */
