#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "w5200_io.h"
#include "spi.h"
#include "delay.h"

#define W52_INT4R RPC0
#define W52_SDI2 RPC3
#define W52_SDO2 RPC1
#define CHN SPI_CHANNEL1

#define CONFIG         (CNC_ON | CNC_IDLE_CON)
#define PINS           (CNC0_ENABLE)
#define PULLUPS        (CNC0_PULLUP_ENABLE)

#define W52_CS_LOW mPORTBClearBits(W52_CS_B3);
#define W52_CS_HIGH mPORTBSetBits(W52_CS_B3);

void wiznet_io_init()
{
    unsigned int temp;
    
    mPORTAClearBits(W52_PORTA);
    mPORTASetPinsDigitalOut(W52_PORTA);

    mPORTBClearBits(W52_PORTB);
    mPORTBSetPinsDigitalOut(W52_PORTB);

    W52_CS_HIGH;
    mPORTBSetBits(W52_RST_B2);
    mPORTAClearBits(W52_PWDN_A8);
    // Add Delay? may or may not need this..
    delay_millis(200);
    wiznet_hrd_rst();
    
    mPORTCClearBits(W52_PORTC);
    mPORTCSetPinsDigitalIn(W52_PORTC);
    mCNCOpen(CONFIG, PINS, PULLUPS);
    PPSInput(1,INT4R,W52_INT4R);
    // Read port to clear mismatch on change notice pins
    // Don't know if I need to do this...
    temp = mPORTCRead();



    INTSetVectorPriority(INT_EXTERNAL_4_VECTOR, INT_PRIORITY_LEVEL_3);
    INTSetVectorSubPriority(INT_EXTERNAL_4_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_INT4, INT_ENABLED);



    PPSInput(3,SDI2, W52_SDI2);  //Assign SDI1 to pin RA1
    PPSOutput(3,W52_SDO2,SDO2); // Set RA2 pin as output for SDO1
}

void wiznet_w_reg(uint16_t addr, uint8_t val)
{
    spi_open(SPI_OPEN_MODE8);

    W52_CS_LOW;
    SpiChnPutC(CHN, addr >> 8);
    SpiChnPutC(CHN, addr & 0xFF);
    SpiChnPutC(CHN, W52_SPI_OPCODE_WRITE >> 8);
    SpiChnPutC(CHN, 0x01);
    SpiChnPutC(CHN, val);
    W52_CS_HIGH;

    CLOSE_SPI;
}

uint8_t wiznet_r_reg(uint16_t addr)
{
    uint8_t val;

    spi_open(SPI_OPEN_MODE8);
    W52_CS_LOW;
    SpiChnPutC(CHN, addr >> 8);
    SpiChnPutC(CHN, addr & 0xFF);
    SpiChnPutC(CHN, W52_SPI_OPCODE_WRITE >> 8);
    SpiChnPutC(CHN, 0x01);
    SpiChnPutC(CHN, 0xFF);
    val = SpiChnGetC(CHN); 
    W52_CS_HIGH;
    CLOSE_SPI;
    
    return val;
}

void wiznet_w_reg16(uint16_t addr, uint16_t val)
{
    spi_open(SPI_OPEN_MODE16);

    W52_CS_LOW;
    SpiChnPutC(CHN, addr);
    SpiChnPutC(CHN, W52_SPI_OPCODE_WRITE | 0x0001);
    SpiChnPutC(CHN, val);
    W52_CS_HIGH;
    CLOSE_SPI;
}

uint16_t wiznet_r_reg16(uint16_t addr)
{
    uint16_t val;

    spi_open(SPI_OPEN_MODE16);
    W52_CS_LOW;
    SpiChnPutC(CHN, addr);
    SpiChnPutC(CHN, W52_SPI_OPCODE_READ | 0x0001);
    SpiChnPutC(CHN, 0xFFFF);
    val = SpiChnGetC(CHN);
    W52_CS_HIGH;
    CLOSE_SPI;

    return val;
}

void wiznet_w_set(uint16_t addr, uint16_t len, uint8_t val)
{
    uint16_t i;

    if (!len) {
        return;
    }

    spi_open(SPI_OPEN_MODE8);
    W52_CS_LOW;
    SpiChnPutC(CHN, addr >> 8);
    SpiChnPutC(CHN, addr & 0xFF);
    SpiChnPutC(CHN, (W52_SPI_OPCODE_WRITE >> 8) | (len >> 8) );
    SpiChnPutC(CHN, len & 0xFF);
    for (i=0; i < len; i++) {
        SpiChnPutC(CHN, val);
    }

    W52_CS_HIGH;
    CLOSE_SPI;
}

void wiznet_w_buf(uint16_t addr, uint16_t len, void *buf)
{
    uint8_t *bufptr = (uint8_t *)buf;
    uint16_t i;

    if (!len) {
        return;
    }

    spi_open(SPI_OPEN_MODE8);
    W52_CS_LOW;
    SpiChnPutC(CHN, addr >> 8);
    SpiChnPutC(CHN, addr & 0xFF);
    SpiChnPutC(CHN, (W52_SPI_OPCODE_WRITE >> 8) | (len >> 8) );
    SpiChnPutC(CHN, len & 0xFF);
    for (i=0; i < len; i++) {
        SpiChnPutC(CHN, bufptr[i]);
    }

    W52_CS_HIGH;
    CLOSE_SPI;
}

void wiznet_r_buf(uint16_t addr, uint16_t len, void *buf)
{
    uint8_t *bufptr = (uint8_t *)buf;
    uint16_t i;

    if (!len) {
        return;
    }

    spi_open(SPI_OPEN_MODE8);
    W52_CS_LOW;
    SpiChnPutC(CHN, addr >> 8);
    SpiChnPutC(CHN, addr & 0xFF);
    SpiChnPutC(CHN, (W52_SPI_OPCODE_READ >> 8) | (len >> 8) );
    SpiChnPutC(CHN, len & 0xFF);
    for (i=0; i < len; i++) {
            SpiChnPutC(CHN, 0xFF);
            bufptr[i] = SpiChnGetC(CHN);
    }

    W52_CS_HIGH;
    CLOSE_SPI;
}

uint16_t wiznet_search_r_buf(uint16_t addr, uint16_t len, void *buf, uint8_t searchchar)
{
    uint8_t *bufptr = (uint8_t *)buf, keep_reading = 1;
    uint16_t i, ttl=0;

    if (!len) {
        return 0;
    }

    spi_open(SPI_OPEN_MODE8);
    W52_CS_LOW;
    SpiChnPutC(CHN, addr >> 8);
    SpiChnPutC(CHN, addr & 0xFF);
    SpiChnPutC(CHN,  (W52_SPI_OPCODE_READ >> 8) | (len >> 8) );
    SpiChnPutC(CHN, len & 0xFF);
    for (i=0; i < len; i++) {
        if (keep_reading) {
            SpiChnPutC(CHN, 0xFF);
            bufptr[i] = SpiChnGetC(CHN);
            ttl++;
            if (bufptr[i] == searchchar)
                keep_reading = 0;
        } else {
            SpiChnPutC(CHN, 0xFF);  // Drain remaining bytes (W5200 doesn't like abrupt cessation of SPI transfers)
        }
    }
    W52_CS_HIGH;
    CLOSE_SPI;

    return ttl;
}
