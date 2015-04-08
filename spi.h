/* 
 * File:   spi.h
 * Author: mtleising
 *
 * Created on April 7, 2015, 4:32 PM
 */

#ifndef SPI_H
#define	SPI_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>

void spi1_init();
void spi2_init();
uint8_t spi1_8(uint8_t data);
uint16_t spi1_16(uint16_t data);
uint8_t spi2_8(uint8_t data);
uint16_t spi2_16(uint16_t data);

#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

