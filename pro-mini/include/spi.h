#ifndef SPI_H
#define SPI_H

#include <stdint.h>

int spi_init(void);
uint8_t spi_transfer(uint8_t data);
#endif
