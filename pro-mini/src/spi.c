#include <avr/io.h>
#include <util/delay.h>
#include "spi.h"

int spi_init(void){
    // Set MOSI, SCK out; MISO in
    DDRB  |= (1 << PB3) | (1 << PB5); // MOSI, SCK
    DDRB  &= ~(1 << PB4);             // MISO

    // Enable SPI, master mode, spi clock = cpu clock/16; SPI Mode CPOL = 0, CPHA = 0 (the default)
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
    return 0;
}

uint8_t spi_transfer(uint8_t data){
    // You put your data in
    SPDR = data;
    // The hardware shifts the data out
    // You wait for data to come in
    while (!(SPSR & (1 << SPIF)));
    // And then we read that data out
    return SPDR;
    // We do the spi-okie pokie
    // and twiddle the bits about
    // that's what it's all about
}
