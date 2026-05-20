#include <avr/io.h>
#include <util/delay.h>
#include "bme280.h"
#include "config.h"
#include "spi.h"

static struct bme280_calib calib;

int bme280_init(){
    spi_init();

    // CS pin for the BME 280, def found in config.h
    DDRD  |= (1 << BME280_CS);

    // CS idle high
    PORTD |= (1 << BME280_CS);
    return 0;
}

int bme280_read(){
    return 0;
}

static void bme280_cs_on(void)  { PORTD &= ~(1 << BME280_CS); }
static void bme280_cs_off(void) { PORTD |=  (1 << BME280_CS); }

// TODO
static uint8_t bme280_read_reg(){
}

// TODO
static void bme280_write_reg(){
}

// TODO
static void bme280_read_burst(){
}

// TODO
static void bme280_load_calib(){
}

// TODO
// BME280 datasheet pg 50 - Temeperature Compensation Formula
static int32_t bme280_compensate_temp(){
}

// TODO
// BME280 datasheet pg 50 - Pressure Compensation Formula
static uint32_t bme280_compensate_pressure(){
}


// TODO
// BME280 datasheet pg 50 - Humidity Compensation Formula
static uint32_t bme280_compensate_humidity(){
}

//TODO Add BME280 self test - BME280 Datasheet pg 53
int bme280_self_test(void){
}

