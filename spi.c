#include <stdint.h>
#include "spi.h"
#include "delay.h"

//bit 15, ON: SPI on bit
//bit 8, CKE: Clock Edge Select bit
//bit 6, CKP: Clock Polarity Select bit
//bit 5, MSTEN: Master Mode Enable bit
#define SPI_8 0x8020
#define SPI_16 0x8420
#define SDI2 0b0111
#define SDO2 0b0111


void spi1_init ()
{
    int rData;

    SPI1CON = 0; // Stops and resets the SPI1.
    rData=SPI1BUF; // clears the receive buffer
    SPI1BRG=0x00; // use FPB/2 clock frequency
    SPI1STATCLR=0x40; // clear the Overflow

    //SDI1R =
}

void spi2_init ()
{
    int rData;

    SPI2CON = 0; // Stops and resets the SPI1.
    rData=SPI1BUF; // clears the receive buffer
    SPI2BRG=0x00; // use FPB/2 clock frequency
    SPI2STATCLR=0x40; // clear the Overflow
}

uint8_t spi1_8(uint8_t data)
{
    SPI1CON = 0x8020;
    SPI2BUF = data;
    while (SPI2STATbits.SPIBUSY);  // wait until SPI transmission complete
    return SPI2BUF;
}

uint16_t spi1_16(uint16_t data)
{
    SPI1CON = 0x8020;
    SPI2BUF = data;
    while (SPI2STATbits.SPIBUSY);  // wait until SPI transmission complete
    return SPI2BUF;
}

uint8_t spi2_8(uint8_t data)
{
    SPI1CON = 0x8020;
    SPI2BUF = data;
    while (SPI2STATbits.SPIBUSY);  // wait until SPI transmission complete
    return SPI2BUF;
}

uint16_t spi2_16(uint16_t data)
{
    SPI1CON = 0x8020;
    SPI2BUF = data;
    while (SPI2STATbits.SPIBUSY);  // wait until SPI transmission complete
    return SPI2BUF;
}
