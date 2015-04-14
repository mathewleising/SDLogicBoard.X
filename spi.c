#include <stdint.h>
#include "spi.h"
#include "delay.h"
#include "config.h"

//bit 15, ON: SPI on bit
//bit 8, CKE: Clock Edge Select bit
//bit 6, CKP: Clock Polarity Select bit
//bit 5, MSTEN: Master Mode Enable bit

#define SPI1_8 SPI1CON = 0x8020
#define SPI1_16 SPI1CON = 0x8420
#define SPI2_8 SPI2CON = 0x8120
#define SPI2_16 SPI2CON = 0x8520

#define SPI1_READY (SPI1STAT & 0x0001)
#define SPI2_READY (SPI2STAT & 0x0001)

void spi_init ()
{
    int rData;
    
    SDI1R = SDI1;
    RPB13R = SDO1;    
    
    SDI2R = SDI2;
    RPC3R = SDO2;
    
    SPI1CON = 0; // Stops and resets the SPI1.
    rData=SPI1BUF; // clears the receive buffer
    SPI1BRG=0x00; // use FPB/2 clock frequency
    SPI1STATCLR=0x40; // clear the Overflow

    SPI2CON = 0; // Stops and resets the SPI1.
    rData=SPI2BUF; // clears the receive buffer
    SPI2BRG=0x00; // use FPB/2 clock frequency
    SPI2STATCLR=0x40; // clear the Overflow

    SPI2_8;
}

uint8_t spi1_8(uint8_t data)
{
    SPI1BUF = data;
    while (!SPI1_READY);  // wait until SPI transmission complete
    return SPI1BUF;
}

uint16_t spi1_16(uint16_t data)
{
    SPI1BUF = data;
    while (!SPI1_READY);  // wait until SPI transmission complete
    return SPI1BUF;
}

uint8_t spi2_8(uint8_t data)
{
    SPI2BUF = data;
    while (!SPI2_READY);  // wait until SPI transmission complete
    return SPI2BUF;
}

uint16_t spi2_16(uint16_t data)
{   
    uint16_t tmp = 0;

    SPI2BUF = data >> 8;
    while (!SPI2_READY);  // wait until SPI transmission complete
    tmp = SPI2BUF << 8;

    SPI2BUF = data & 0xFF;
    while (!SPI2_READY);  // wait until SPI transmission complete
    tmp = tmp | (SPI2BUF & 0xFF);

    return tmp;
}
