#ifndef I2C_H
#define I2C_H

#define ZONE_COUNT 3
#define ZONE_1_ADDR 0x10
#define ZONE_2_ADDR 0x11
#define ZONE_3_ADDR 0x12

int i2c_init(void);
int i2c_poll_zones(void);

#endif /* I2C_H */
