#include <stdint.h>
#include "config.h"
#include "max110.h"
#include "delay.h"

#define SPI1_8 SPI1CON = 0x8020
#define SPI1_16 SPI1CON = 0x8420
#define SPI1_32 SPI1CON = 0x8820

#define SPI8(data) spi1_8(data)
#define SPI16(data) spi1_16(data)
#define SPI32(data) spi1_32(data)

void max110_init(void)
{
    ADC_CS_STOP;

    delay_millis(10);

    SPI1_8;
    ADC_CS_START;
    SPI8(MAX110_WRITE | R_CONFIG);
    SPI8(MAX110_RST);
    ADC_CS_STOP;

    delay_millis(10);

    ADC_CS_START;
    SPI8(MAX110_WRITE | R_CONFIG);
    SPI8(CONFIG);
    ADC_CS_STOP;
}

void get_data(MAX110_d *data)
{
    uint32_t tmp[3];

    SPI1_8;

    ADC_CS_START;
    
    SPI8(R_DATA);

    SPI1_32;
    tmp[0] = SPI32(0x000000);
    tmp[1] = SPI32(0x000000);
    tmp[2] = SPI32(0x000000);

    data->data_1 = (uint16_t)((tmp[0]>>8)&0xFFFF);
    data->data_2 = (uint16_t)(tmp[1]&0xFFFF);
    data->data_3 = (uint16_t)((tmp[1]>>24)|(tmp[3]&0xFF));
    data->data_4 = (uint16_t)((tmp[3]>>16)&0xFFFF);

    ADC_CS_STOP;
}