#include <avr/io.h>
#include <util/delay.h>
#include "bme280.h"
#include "config.h"

static struct bme280_calib calib;

/* SPI helper functions */
static void spi_init(void)
{
    /* Set MOSI, SCK, CS as outputs; MISO as input */
    DDRB  |= (1 << PB3) | (1 << PB5);  /* MOSI, SCK */
    DDRB  &= ~(1 << PB4);              /* MISO */
    DDRD  |= (1 << BME280_CS_PIN);     /* CS */

    /* CS idle high */
    PORTD |= (1 << BME280_CS_PIN);

    /* Enable SPI, master mode, clock/16 */
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

static uint8_t spi_transfer(uint8_t data)
{
    SPDR = data;
    while (!(SPSR & (1 << SPIF)));
    return SPDR;
}

static void bme280_cs_low(void)  { PORTD &= ~(1 << BME280_CS_PIN); }
static void bme280_cs_high(void) { PORTD |=  (1 << BME280_CS_PIN); }

static uint8_t bme280_read_reg(uint8_t reg)
{
    bme280_cs_low();
    spi_transfer(reg | 0x80);   /* set read bit */
    uint8_t val = spi_transfer(0x00);
    bme280_cs_high();
    return val;
}

static void bme280_write_reg(uint8_t reg, uint8_t val)
{
    bme280_cs_low();
    spi_transfer(reg & 0x7F);   /* clear write bit */
    spi_transfer(val);
    bme280_cs_high();
}

static void bme280_read_burst(uint8_t reg, uint8_t *buf, uint8_t len)
{
    bme280_cs_low();
    spi_transfer(reg | 0x80);
    for (uint8_t i = 0; i < len; i++) {
        buf[i] = spi_transfer(0x00);
    }
    bme280_cs_high();
}

static void bme280_load_calib(void)
{
    uint8_t buf[26];

    /* Read temperature and pressure calibration (0x88-0xA1) */
    bme280_read_burst(BME280_REG_CALIB_00, buf, 26);

    calib.dig_T1 = (uint16_t)(buf[1]  << 8) | buf[0];
    calib.dig_T2 = (int16_t) (buf[3]  << 8) | buf[2];
    calib.dig_T3 = (int16_t) (buf[5]  << 8) | buf[4];

    calib.dig_P1 = (uint16_t)(buf[7]  << 8) | buf[6];
    calib.dig_P2 = (int16_t) (buf[9]  << 8) | buf[8];
    calib.dig_P3 = (int16_t) (buf[11] << 8) | buf[10];
    calib.dig_P4 = (int16_t) (buf[13] << 8) | buf[12];
    calib.dig_P5 = (int16_t) (buf[15] << 8) | buf[14];
    calib.dig_P6 = (int16_t) (buf[17] << 8) | buf[16];
    calib.dig_P7 = (int16_t) (buf[19] << 8) | buf[18];
    calib.dig_P8 = (int16_t) (buf[21] << 8) | buf[20];
    calib.dig_P9 = (int16_t) (buf[23] << 8) | buf[22];

    calib.dig_H1 = buf[25];

    /* Read humidity calibration (0xE1-0xE7) */
    uint8_t hbuf[7];
    bme280_read_burst(BME280_REG_CALIB_26, hbuf, 7);

    calib.dig_H2 = (int16_t) (hbuf[1] << 8) | hbuf[0];
    calib.dig_H3 = hbuf[2];
    calib.dig_H4 = (int16_t) (hbuf[3] << 4) | (hbuf[4] & 0x0F);
    calib.dig_H5 = (int16_t) (hbuf[5] << 4) | (hbuf[4] >> 4);
    calib.dig_H6 = (int8_t)   hbuf[6];
}

/* Compensation formulas from BME280 datasheet */
static int32_t bme280_compensate_temp(int32_t adc_T)
{
    int32_t var1, var2;
    var1 = ((((adc_T >> 3) - ((int32_t)calib.dig_T1 << 1)))
            * (int32_t)calib.dig_T2) >> 11;
    var2 = (((((adc_T >> 4) - (int32_t)calib.dig_T1)
            * ((adc_T >> 4) - (int32_t)calib.dig_T1)) >> 12)
            * (int32_t)calib.dig_T3) >> 14;
    calib.t_fine = var1 + var2;
    return (calib.t_fine * 5 + 128) >> 8;  /* returns temp in 0.01 DegC */
}

static uint32_t bme280_compensate_pressure(int32_t adc_P)
{
    int64_t var1, var2, p;
    var1 = (int64_t)calib.t_fine - 128000;
    var2 = var1 * var1 * (int64_t)calib.dig_P6;
    var2 = var2 + ((var1 * (int64_t)calib.dig_P5) << 17);
    var2 = var2 + (((int64_t)calib.dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)calib.dig_P3) >> 8)
         + ((var1 * (int64_t)calib.dig_P2) << 12);
    var1 = (((int64_t)1 << 47) + var1) * (int64_t)calib.dig_P1 >> 33;
    if (var1 == 0) return 0;
    p    = 1048576 - adc_P;
    p    = (((p << 31) - var2) * 3125) / var1;
    var1 = ((int64_t)calib.dig_P9 * (p >> 13) * (p >> 13)) >> 25;
    var2 = ((int64_t)calib.dig_P8 * p) >> 19;
    p    = ((p + var1 + var2) >> 8) + ((int64_t)calib.dig_P7 << 4);
    return (uint32_t)p;   /* returns pressure in Q24.8 format (Pa * 256) */
}

static uint32_t bme280_compensate_humidity(int32_t adc_H)
{
    int32_t v;
    v = calib.t_fine - 76800;
    v = (((((adc_H << 14) - ((int32_t)calib.dig_H4 << 20)
        - ((int32_t)calib.dig_H5 * v)) + 16384) >> 15)
        * (((((((v * (int32_t)calib.dig_H6) >> 10)
        * (((v * (int32_t)calib.dig_H3) >> 11) + 32768)) >> 10)
        + 2097152) * (int32_t)calib.dig_H2 + 8192) >> 14));
    v = v - (((((v >> 15) * (v >> 15)) >> 7)
        * (int32_t)calib.dig_H1) >> 4);
    v = v < 0 ? 0 : v;
    v = v > 419430400 ? 419430400 : v;
    return (uint32_t)(v >> 12);  /* returns humidity in Q22.10 format (% * 1024) */
}

int bme280_init(void)
{
    spi_init();

    /* Verify chip ID */
    uint8_t id = bme280_read_reg(BME280_REG_ID);
    if (id != BME280_CHIP_ID) {
        return -1;
    }

    /* Reset the sensor */
    bme280_write_reg(BME280_REG_RESET, BME280_RESET_VALUE);
    _delay_ms(10);

    /* Load calibration data */
    bme280_load_calib();

    /* Set humidity oversampling x1 (must be set before CTRL_MEAS) */
    bme280_write_reg(BME280_REG_CTRL_HUM, BME280_OS_1X);

    /* Set temp and pressure oversampling x1, normal mode */
    bme280_write_reg(BME280_REG_CTRL_MEAS,
                     (BME280_OS_1X << 5) |
                     (BME280_OS_1X << 2) |
                      BME280_MODE_NORMAL);

    return 0;
}

int bme280_read(int16_t *temperature, uint16_t *humidity, uint32_t *pressure)
{
    uint8_t buf[8];

    /* Burst read all raw data registers (0xF7-0xFE) */
    bme280_read_burst(BME280_REG_PRESS_MSB, buf, 8);

    int32_t adc_P = ((int32_t)buf[0] << 12) | ((int32_t)buf[1] << 4) | (buf[2] >> 4);
    int32_t adc_T = ((int32_t)buf[3] << 12) | ((int32_t)buf[4] << 4) | (buf[5] >> 4);
    int32_t adc_H = ((int32_t)buf[6] << 8)  |  (int32_t)buf[7];

    /* Compensate using datasheet formulas — order matters, temp must go first
     * as it sets t_fine which pressure and humidity compensation depend on */
    int32_t temp = bme280_compensate_temp(adc_T);
    *temperature = (int16_t)(temp / 10);        /* convert 0.01C to 0.1C scaling */
    *pressure    = bme280_compensate_pressure(adc_P) >> 8;  /* convert to whole Pascals */
    *humidity    = (uint16_t)(bme280_compensate_humidity(adc_H) * 10 / 1024); /* convert to 0.1% scaling */

    return 0;
}
