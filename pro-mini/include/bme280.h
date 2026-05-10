#ifndef BME280_H
#define BME280_H

#include <stdint.h>

/* BME280 registers */
#define BME280_REG_ID           0xD0
#define BME280_REG_RESET        0xE0
#define BME280_REG_CTRL_HUM     0xF2
#define BME280_REG_STATUS       0xF3
#define BME280_REG_CTRL_MEAS    0xF4
#define BME280_REG_CONFIG       0xF5
#define BME280_REG_PRESS_MSB    0xF7
#define BME280_REG_CALIB_00     0x88
#define BME280_REG_CALIB_26     0xE1

#define BME280_CHIP_ID          0x60
#define BME280_RESET_VALUE      0xB6

/* Oversampling settings */
#define BME280_OS_1X            0x01
#define BME280_OS_2X            0x02
#define BME280_OS_4X            0x03
#define BME280_OS_8X            0x04
#define BME280_OS_16X           0x05

/* Mode settings */
#define BME280_MODE_SLEEP       0x00
#define BME280_MODE_FORCED      0x01
#define BME280_MODE_NORMAL      0x03

/* Calibration data structure */
struct bme280_calib {
    /* Temperature */
    uint16_t dig_T1;
    int16_t  dig_T2;
    int16_t  dig_T3;

    /* Pressure */
    uint16_t dig_P1;
    int16_t  dig_P2;
    int16_t  dig_P3;
    int16_t  dig_P4;
    int16_t  dig_P5;
    int16_t  dig_P6;
    int16_t  dig_P7;
    int16_t  dig_P8;
    int16_t  dig_P9;

    /* Humidity */
    uint8_t  dig_H1;
    int16_t  dig_H2;
    uint8_t  dig_H3;
    int16_t  dig_H4;
    int16_t  dig_H5;
    int8_t   dig_H6;

    /* Fine temperature value shared across compensation formulas */
    int32_t  t_fine;
};

int bme280_init(void);
int bme280_read(int16_t *temperature, uint16_t *humidity, uint32_t *pressure);

#endif /* BME280_H */
