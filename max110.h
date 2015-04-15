/* 
 * File:   max110.h
 * Author: mathewleising
 *
 * Created on April 15, 2015, 2:44 PM
 */

#ifndef MAX110_H
#define	MAX110_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>

#define MAX110_READ 0x80
#define MAX110_WRITE 0x00

#define R_SMPL_INST 0b1000000 // 32 bits
#define R_DATA_RATE_CNT 0b1010000 // 16 bits
#define R_CONFIG 0b1100000 // 8 bits
#define R_DATA 0b1110000 // 96 bits

#define MAX110_RST 0x40
#define SMPL_INST 0x000000
#define CONFIG 0x10
#define DATA_RATE_CNT 0x0000

typedef struct {
    uint16_t data_1;
    uint16_t data_2;
    uint16_t data_3;
    uint16_t data_4;
} MAX110_d;

void max110_init(void);
void get_data(MAX110_d *data);

#ifdef	__cplusplus
}
#endif

#endif	/* MAX110_H */

