#ifndef BME280_H
#define BME280_H

#include <stdint.h>

// BME 280 Register Address Map
#define BME280_ID_REG 0xD0
#define BME280_CONFIG 0xF5
#define HUM_LSB 0xFE
#define HUM_MSB 0xFD
#define TEMP_XLSB 0xFC
#define TEMP_LSB 0xFB
#define TEMP_MSB 0xFA
#define PRESS_XLSB 0xF9
#define PRESS_LSB 0xF8
#define PRESS_MSB 0xF7
#define CTRL_MEAS 0xF4
#define BME280_STATUS 0xF3
#define CTRL_HUM 0xF2
#define BME280_RESET_REG 0xE0

// The reading the chip ID register should return the following value
#define BME280_ID 0x60
// place this value into the BME280_RESET_REG to trigger a reset of the chip
#define BME280_SOFT_RESET 0xB6

#define T_SB0 5
#define T_SB1 6
#define T_SB2 7

#define BME_280_FILTER0 2
#define BME_280_FILTER1 3
#define BME_280_FILTER2 4

#define SPI3W_EN 0

#define OSRS_T0 5
#define OSRS_T1 6
#define OSRS_T2 7

#define OSRS_P0 2
#define OSRS_P1 3
#define OSRS_P2 4

#define OSRS_H0 0
#define OSRS_H1 1
#define OSRS_H2 2

#define BME280_MEASURING 3
#define IM_UPDATE 0

#define BME280_MODE0 0
#define BME280_MODE1 1
// temperature calibration registers
#define DIG_T1_LSB 0x88
#define DIG_T1_MSB 0x89
#define DIG_T2_LSB 0x8A
#define DIG_T2_MSB 0x8B
#define DIG_T3_LSB 0x8C
#define DIG_T3_MSB 0x8D

// pressure calibration registers
#define DIG_P1_LSB 0x8E
#define DIG_P1_MSB 0x8F
#define DIG_P2_LSB 0x90
#define DIG_P2_MSB 0x91
#define DIG_P3_LSB 0x92
#define DIG_P3_MSB 0x93
#define DIG_P4_LSB 0x94
#define DIG_P4_MSB 0x95
#define DIG_P5_LSB 0x96
#define DIG_P5_MSB 0x97
#define DIG_P6_LSB 0x98
#define DIG_P6_MSB 0x99
#define DIG_P7_LSB 0x9A
#define DIG_P7_MSB 0x9B
#define DIG_P8_LSB 0x9C
#define DIG_P8_MSB 0x9D
#define DIG_P9_LSB 0x9E
#define DIG_P9_MSB 0x9F

// humidity calibration registers
// TODO
#define DIG_H1 0xA1
#define DIG_H2_LSB 0xE1
#define DIG_H2_MSB 0xE2
#define DIG_H3 0xE3
// The following two values have a bit of a funky mapping as they share half of the E5 register
// [i:j]REG => [k:l]NUM signifies that bits i to j of register map to bits k to l of the number
#define DIG_H4_MSB 0xE4
#define DIG_H4_LSB 0xE5 // bits [3:0]REG => [3:0]NUM
#define DIG_H5_LSB 0xE5 // bits [7:4]REG => [3:0]NUM
#define DIG_H5_MSB 0xE6
#define DIG_H6 0xE7

struct dig_t{
    uint16_t T1;
    int16_t T2;
    int16_t T3;
};

struct dig_p{
    uint16_t P1;
    int16_t P2;
    int16_t P3;
    int16_t P4;
    int16_t P5;
    int16_t P6;
    int16_t P7;
    int16_t P8;
    int16_t P9;
};

struct dig_h{
    uint8_t H1;
    int16_t H2;
    uint8_t H3;
    int16_t H4;
    int16_t H5;
    int8_t H6;
};

struct bme280_calib{
    struct dig_t temp;
    struct dig_p press;
    struct dig_h hum;
};

int bme280_init();
int bme280_read();
int bme280_self_test(void);

#endif
