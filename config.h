/* 
 * File:   config.h
 * Author: mtleising
 *
 * Created on April 3, 2015, 5:22 PM
 */

#ifndef CONFIG_H
#define	CONFIG_H

#include <p32xxxx.h>

#ifdef	__cplusplus
extern "C" {
#endif
    
#define SYS_FREQ (40000000L) // (48000000L)

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

// External Interupt
#define EX_CON 0x1000

// Peripheral Pin Select
#define ADC_DATA_READY 0b0110
#define ETH_INT 0b0110
#define ADC_FLT_INT 0b0101
#define ADC_OFLW_INT 0b0100
#define SDI1 0b0011
#define SDO1 0b0011
#define SDI2 0b0111
#define SDO2 0b0100

// May not need any inputs (all are pph)
#define PORTA_CONFIG TRISA = 0x0000
#define PORTB_CONFIG TRISB = 0x0A00 // 0x0000 PPS Inputs
#define PORTC_CONFIG TRISC = 0x01C9 // 0x0080 PPS Inputs

// Output Ports
#define LED1_R_ON LATBbits.LATB7 = 1
#define LED1_R_OFF LATBbits.LATB7 = 0

#define LED1_G_ON LATBbits.LATB8 = 1
#define LED1_G_OFF LATBbits.LATB8 = 0

#define LED2_R_ON LATCbits.LATC4 = 1
#define LED2_R_OFF LATCbits.LATC4 = 0

#define LED2_G_ON LATCbits.LATC5 = 1
#define LED2_G_OFF LATCbits.LATC5 = 0

#define W5200_PWR_ON LATAbits.LATA8 = 0
#define W5200_PWR_OFF LATAbits.LATA8 = 1

#define W5200_CS_START LATBbits.LATB3 = 0
#define W5200_CS_STOP LATBbits.LATB3 = 1

#define W5200_RST_START LATBbits.LATB2 = 0
#define W5200_RST_STOP LATBbits.LATB2 = 1

#define ADC_CS_START LATBbits.LATB10 = 0
#define ADC_CS_STOP LATBbits.LATB10 = 1

// Input Ports
    

#ifdef	__cplusplus
}
#endif

#endif	/* CONFIG_H */

