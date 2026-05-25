#include "bme280.h"
#include "config.h"
#include "spi.h"
#include "uart.h"
#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <util/delay.h>

static struct bme280_calib calib; // a set of manufacturer loaded calibration values specific to each chip, used in
                                  // temperature, pressure, and humidity compensation formulas
static int32_t t_fine;            // a variable used in temperature, pressure, and humidity compensation formulas

static void bme280_cs_on(void) { PORTD &= ~(1 << BME280_CS); }
static void bme280_cs_off(void) { PORTD |= (1 << BME280_CS); }

static uint8_t bme280_read_reg(uint8_t reg) {
    uint8_t data;
    bme280_cs_on();
    spi_transfer(reg | (1 << 7)); // set bit 7 high for read
    data = spi_transfer(0x00);
    bme280_cs_off();
    return data;
}

static void bme280_write_reg(uint8_t reg, uint8_t reg_value) {
    bme280_cs_on();
    spi_transfer(reg & ~(1 << 7));
    spi_transfer(reg_value);
    bme280_cs_off();
}

static void bme280_burst_read(uint8_t start_reg, uint8_t *buffer, uint8_t length) {
    bme280_cs_on();
    spi_transfer(start_reg | (1 << 7)); // set bit 7 high for read
    for (uint8_t i = 0; i < length; i++) {
        buffer[i] = spi_transfer(0x00);
    }
    bme280_cs_off();
}

static void bme280_load_calib() {
    // temperature and pressure calibration data are in one contiguous register map
    uint8_t temp_press_buf[DIG_P9_MSB - DIG_T1_LSB + 1];
    bme280_burst_read(DIG_T1_LSB, temp_press_buf, DIG_P9_MSB - DIG_T1_LSB + 1);

    // H1 calibration data is floating off on its own
    calib.hum.H1 = bme280_read_reg(DIG_H1);

    // The rest of the humidity calibration data is in another register block
    uint8_t hum_buf[DIG_H6 - DIG_H2_LSB + 1];
    bme280_burst_read(DIG_H2_LSB, hum_buf, DIG_H6 - DIG_H2_LSB + 1);

    // Temperature calibration
    calib.temp.T1 = (((uint16_t)temp_press_buf[1] << 8) | temp_press_buf[0]);
    calib.temp.T2 = (((int16_t)temp_press_buf[3] << 8) | temp_press_buf[2]);
    calib.temp.T3 = (((int16_t)temp_press_buf[5] << 8) | temp_press_buf[4]);

    // Pressure calibration
    calib.press.P1 = (((uint16_t)temp_press_buf[7] << 8) | temp_press_buf[6]);
    calib.press.P2 = (((int16_t)temp_press_buf[9] << 8) | temp_press_buf[8]);
    calib.press.P3 = (((int16_t)temp_press_buf[11] << 8) | temp_press_buf[10]);
    calib.press.P4 = (((int16_t)temp_press_buf[13] << 8) | temp_press_buf[12]);
    calib.press.P5 = (((int16_t)temp_press_buf[15] << 8) | temp_press_buf[14]);
    calib.press.P6 = (((int16_t)temp_press_buf[17] << 8) | temp_press_buf[16]);
    calib.press.P7 = (((int16_t)temp_press_buf[19] << 8) | temp_press_buf[18]);
    calib.press.P8 = (((int16_t)temp_press_buf[21] << 8) | temp_press_buf[20]);
    calib.press.P9 = (((int16_t)temp_press_buf[23] << 8) | temp_press_buf[22]);

    // Humidity calibration
    calib.hum.H2 = (((int16_t)hum_buf[1] << 8) | hum_buf[0]);
    calib.hum.H3 = hum_buf[2];
    calib.hum.H4 = (((int16_t)hum_buf[3] << 4) | (hum_buf[4] & 0x0F));
    calib.hum.H5 = (((int16_t)hum_buf[5] << 4) | ((hum_buf[4] >> 4) & 0x0F));
    calib.hum.H6 = (int8_t)hum_buf[6];
}

// BME280 datasheet pg 50 - Temeperature Compensation Formula
static int32_t bme280_compensate_temp(int32_t raw_temp_value) {
    int32_t alpha, beta, corrected_temp;
    alpha = ((((raw_temp_value >> 3) - ((int32_t)calib.temp.T1 << 1))) * ((int32_t)calib.temp.T2)) >> 11;
    beta =
        (((((raw_temp_value >> 4) - ((int32_t)calib.temp.T1)) * ((raw_temp_value >> 4) - ((int32_t)calib.temp.T1))) >>
          12) *
         ((int32_t)calib.temp.T3)) >>
        14;
    t_fine = alpha + beta;
    corrected_temp = (t_fine * 5 + 128) >> 8;
    return corrected_temp;
}

// BME280 datasheet pg 50 - Pressure Compensation Formula
static uint32_t bme280_compensate_pressure(int32_t raw_pressure_value) {
    int32_t gamma, delta;
    uint32_t corrected_pressure;
    gamma = (((int32_t)t_fine) >> 1) - (int32_t)64000;
    delta = (((gamma >> 2) * (gamma >> 2)) >> 11) * ((int32_t)calib.press.P6);
    delta = delta + ((gamma * ((int32_t)calib.press.P5)) << 1);
    delta = (delta >> 2) + (((int32_t)calib.press.P4) << 16);
    gamma = (((calib.press.P3 * (((gamma >> 2) * (gamma >> 2)) >> 13)) >> 3) +
             ((((int32_t)calib.press.P2) * gamma) >> 1)) >>
            18;
    gamma = ((((32768 + gamma)) * ((int32_t)calib.press.P1)) >> 15);
    if (gamma == 0) {
        return 0;
    }
    corrected_pressure = (((uint32_t)(((int32_t)1048576) - raw_pressure_value) - (delta >> 12))) * 3125;
    if (corrected_pressure < 0x80000000) {
        corrected_pressure = (corrected_pressure << 1) / ((uint32_t)gamma);
    } else {
        corrected_pressure = (corrected_pressure / (uint32_t)gamma) * 2;
    }
    gamma =
        (((int32_t)calib.press.P9) * ((int32_t)(((corrected_pressure >> 3) * (corrected_pressure >> 3)) >> 13))) >> 12;
    delta = (((int32_t)(corrected_pressure >> 2)) * ((int32_t)calib.press.P8)) >> 13;
    corrected_pressure = (uint32_t)((int32_t)corrected_pressure + ((gamma + delta + calib.press.P7) >> 4));
    return corrected_pressure;
}

// BME280 datasheet pg 25 - Humidity Compensation Formula
static uint32_t bme280_compensate_humidity(int32_t raw_humidity_value) {
    int32_t corrected_humidity;
    corrected_humidity = (t_fine - ((int32_t)76800));
    corrected_humidity = (((((raw_humidity_value << 14) - (((int32_t)calib.hum.H4) << 20) -
                             (((int32_t)calib.hum.H5) * corrected_humidity)) +
                            ((int32_t)16384)) >>
                           15) *
                          (((((((corrected_humidity * ((int32_t)calib.hum.H6)) >> 10) *
                               (((corrected_humidity * ((int32_t)calib.hum.H3)) >> 11) + ((int32_t)32768))) >>
                              10) +
                             ((int32_t)2097152)) *
                                ((int32_t)calib.hum.H2) +
                            8192) >>
                           14));
    corrected_humidity =
        (corrected_humidity -
         (((((corrected_humidity >> 15) * (corrected_humidity >> 15)) >> 7) * ((int32_t)calib.hum.H1)) >> 4));
    corrected_humidity = (corrected_humidity < 0 ? 0 : corrected_humidity);
    corrected_humidity = (corrected_humidity > 419430400 ? 419430400 : corrected_humidity);
    return (uint32_t)(corrected_humidity >> 12);
}

uint8_t bme280_init() {
    spi_init();
    struct bme280_data self_test_data;

    // CS pin for the BME 280, def found in config.h
    DDRD |= (1 << BME280_CS);

    // CS idle high
    PORTD |= (1 << BME280_CS);
    // Soft reset the BME280
    bme280_write_reg(BME280_RESET_REG, BME280_SOFT_RESET);
    _delay_ms(100);

    // Check the chip ID
    if (bme280_read_reg(BME280_ID_REG) != BME280_ID) {
        return 10;
    }

    bme280_load_calib();

    bme280_write_reg(CTRL_HUM, ((1 << OSRS_H2) | (1 << OSRS_H0)));
    bme280_write_reg(CTRL_MEAS, ((1 << OSRS_T2) | (1 << OSRS_T0) | (1 << OSRS_P2) | (1 << OSRS_P0)));

    // First read is garabage for some reason, discard first use second, sensor behaves fine after that
    bme280_read(&self_test_data);
    bme280_read(&self_test_data);
    if ((self_test_data.temperature > 5500) || (self_test_data.temperature < 0)) {
        return 40;
    }
    if ((self_test_data.pressure < 90000) || (self_test_data.pressure > 110000)) {
        return 41;
    }
    if ((self_test_data.humidity / 1024 < 9) || (self_test_data.humidity / 1024 > 100)) {
        return 42;
    }
    return 0;
    // TODO finish the self test (codes 20, 30, and 31 not implemented yet
}

uint8_t bme280_read(struct bme280_data *data) {
    int32_t raw_temperature, raw_pressure, raw_humidity;
    uint8_t data_buffer[8];
    bme280_write_reg(CTRL_HUM, ((1 << OSRS_H2) | (1 << OSRS_H0)));
    bme280_write_reg(CTRL_MEAS,
                     ((1 << OSRS_T2) | (1 << OSRS_T0) | (1 << OSRS_P2) | (1 << OSRS_P0) |
                      (1 << BME280_MODE0))); // need to make sure to preserve the oversampling settings even though
                                             // we're just trying to start a forced read
    while (bme280_read_reg(BME280_STATUS) & (1 << BME280_MEASURING))
        ;

    bme280_burst_read(PRESS_MSB, data_buffer, 8);
    raw_pressure = ((int32_t)data_buffer[0] << 12) | ((int32_t)data_buffer[1] << 4) | ((int32_t)data_buffer[2] >> 4);

    raw_temperature = ((int32_t)data_buffer[3] << 12) | ((int32_t)data_buffer[4] << 4) | ((int32_t)data_buffer[5] >> 4);

    raw_humidity = ((int32_t)data_buffer[6] << 8) | (int32_t)data_buffer[7];

    data->temperature = bme280_compensate_temp(raw_temperature);
    data->pressure = bme280_compensate_pressure(raw_pressure);
    data->humidity = bme280_compensate_humidity(raw_humidity);
    return 0;
}
