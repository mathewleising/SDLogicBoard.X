/* 
 * File:   config.h
 * Author: mtleising
 *
 * Created on April 3, 2015, 5:22 PM
 */

#ifndef CONFIG_H
#define	CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif
#define SYS_FREQ (48000000L)

#define FOSC SYS_FREQ
#define PB_DIV 1 // NOTE THIS MAY CHANGE
#define PRESCALE 64
//Last # equals ticks, IE xticks @ FOSC/PB_DIV/PRESCALE. Page 155
#define T1_TICK (FOSC/PB_DIV/PRESCALE/64)

#define PBCLK_FREQUENCY SYS_FREQ/PB_DIV
#define LED_R1_B7 BIT_7
#define LED_G1_B8 BIT_8
#define LED_PORTB (LED_R1_B7 | LED_G1_B8)

#define LED_R2_C4 BIT_4
#define LED_G2_C5 BIT_5
#define LED_PORTC (LED_R2_C4 | LED_G2_C5)



#ifdef	__cplusplus
}
#endif

#endif	/* CONFIG_H */

