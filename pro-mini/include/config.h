#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

/* Zone I2C address is set at compile time via -DZONE_ADDR */
#ifndef ZONE_ADDR
#error "ZONE_ADDR must be defined at compile time"
#endif

/* Subzone and sensor counts */
#define SUBZONE_COUNT    4
#define SENSOR_COUNT     4

/* Pin definitions */
#define FAN_PWM_PIN      PD3   /* D3  - OC2B - Timer2 */
#define LIGHTS_PWM_PIN   PB1   /* D9  - OC1A - Timer1 */

#define VALVE_1_PIN      PD4   /* D4 */
#define VALVE_2_PIN      PD6   /* D6 */
#define VALVE_3_PIN      PD7   /* D7 */
#define VALVE_4_PIN      PB0   /* D8 */

/* BME280 via hardware SPI */
#define BME280_CS_PIN    PD2   /* D2 */
/* MOSI D11, MISO D12, SCK D13 are implicit hardware SPI pins */

/* UART for MH-Z19 CO2 sensor */
#define BAUD_RATE        9600

#endif /* CONFIG_H */
