#include <stdint.h>
#include "config.h"
#include "w5200_io.h"
#include "spi.h"
#include "delay.h"

#define W52_INT4R RPC0
#define W52_SDI2 RPC3
#define W52_SDO2 RPC1
#define CHN SPI_CHANNEL1

#define SPI8(data) spi2_8(data)
#define SPI16(data) spi2_16(data)

void wiznet_w_reg(uint16_t addr, uint8_t val)
{
    W5200_CS_START;
    SPI8(addr >> 8);
    SPI8(addr & 0xFF);
    SPI8(W52_SPI_OPCODE_WRITE >> 8);
    SPI8(0x01);
    SPI8(val);
    W5200_CS_STOP;
}

uint8_t wiznet_r_reg(uint16_t addr)
{
    uint8_t val;

    W5200_CS_START;
    SPI8(addr >> 8);
    SPI8(addr & 0xFF);
    SPI8(W52_SPI_OPCODE_READ >> 8);
    SPI8(0x01);
    val = SPI8(0x00);
    W5200_CS_STOP;
    
    return val;
}

void wiznet_w_reg16(uint16_t addr, uint16_t val)
{
    W5200_CS_START;
    SPI16(addr);
    SPI16(W52_SPI_OPCODE_WRITE | 0x0002);
    SPI16(val);
    W5200_CS_STOP;
}

uint16_t wiznet_r_reg16(uint16_t addr)
{
    uint16_t val;

    W5200_CS_START;
    SPI16(addr);
    SPI16(W52_SPI_OPCODE_READ | 0x0002);
    val = SPI16(0x0000);
    W5200_CS_STOP;

    return val;
}

void wiznet_w_set(uint16_t addr, uint16_t len, uint8_t val)
{
    uint16_t i;

    if (!len) {
        return;
    }

    W5200_CS_START;
    SPI8(addr >> 8);
    SPI8(addr & 0xFF);
    SPI8((W52_SPI_OPCODE_WRITE >> 8) | (len >> 8) );
    SPI8(len & 0xFF);
    for (i=0; i < len; i++) {
        SPI8(val);
    }

    W5200_CS_STOP;
}

void wiznet_w_buff(uint16_t addr, uint8_t *buf, uint16_t len)
{
    uint16_t i;

    if (!len) {
        return;
    }

    W5200_CS_START;
    SPI8(addr >> 8);
    SPI8(addr & 0xFF);
    SPI8((W52_SPI_OPCODE_WRITE >> 8) | (len >> 8) );
    SPI8(len & 0xFF);
    for (i=0; i < len; i++) {
        SPI8(buf[i]);
    }

    W5200_CS_STOP;
}

void wiznet_r_buff(uint16_t addr, uint8_t *buf, uint16_t len)
{
    uint16_t i;

    if (!len) {
        return;
    }

    W5200_CS_START;
    SPI8(addr >> 8);
    SPI8(addr & 0xFF);
    SPI8((W52_SPI_OPCODE_READ >> 8) | (len >> 8) );
    SPI8(len & 0xFF);
    for (i=0; i < len; i++) {
            buf[i] = SPI8(0x00);
    }

    W5200_CS_STOP;
}
