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

#define SPI1_8 SPI1CON = 0x8020
#define SPI1_16 SPI1CON = 0x8420
#define SPI2_8 SPI2CON = 0x8020
#define SPI2_16 SPI2CON = 0x8420

void spi1_init(void);
void spi2_init(void);
uint8_t spi1_8(uint8_t data);
uint16_t spi1_16(uint16_t data);
uint8_t spi2_8(uint8_t data);
uint16_t spi2_16(uint16_t data);

#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

