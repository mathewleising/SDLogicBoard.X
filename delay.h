/* 
 * File:   delay.h
 * Author: mtleising
 *
 * Created on April 3, 2015, 5:21 PM
 */

#ifndef DELAY_H
#define	DELAY_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "config.h"

void delay_for_1000_nops_x (uint16_t n);
void delay_for_1000_nops   ();
void delay_micros          (uint16_t n);
void delay_millis          (uint16_t n);
void delay_seconds         (uint16_t n);

#ifdef	__cplusplus
}
#endif

#endif	/* DELAY_H */

