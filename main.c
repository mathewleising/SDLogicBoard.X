/* 
 * File:   main.c
 * Author: mtleising
 *
 * Created on March 25, 2015, 11:16 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <plib.h>
#include "config.h"
#include "w5200_config.h"
#include "w5200_buf.h"
#include "w5200_sock.h"

#define _SUPPRESS_PLIB_WARNING 
// Configuration Bit settings
// SYSCLK = 48 MHz (8MHz Crystal / FPLLIDIV * FPLLMUL / FPLLODIV)
// PBCLK = 48 MHz (SYSCLK / FPBDIV)
// Primary Osc w/PLL (XT+,HS+,EC+PLL)
// WDT OFF
// Other options are don't care
//#pragma config FPLLMUL = MUL_24, FPLLIDIV = DIV_2, FPLLODIV = DIV_2, FWDTEN = OFF
// 40MHz
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_2, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_1

// Function prototypes
void sysyem_initialize(void);

int main(void)
{
    SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);

    sysyem_initialize();

    while(1);

    return 1;
}

void sysyem_initialize(void)
{
    // General Config
    // May not need this...
    SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);

    mPORTBClearBits(LED_PORTB);
    mPORTBSetPinsDigitalOut(LED_PORTB);

    mPORTCClearBits(LED_PORTB);
    mPORTCSetPinsDigitalOut(LED_PORTB);

    // Interrupts
    // Timer1
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_64, T1_TICK);
    ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_1);

    INTEnableSystemMultiVectoredInt();
    //INTEnableInterrupts();
}

void __ISR(_TIMER_1_VECTOR, ipl7) _Timer1Handler(void)
{
    // clear the interrupt flag
    mT1ClearIntFlag();
    // .. things to do ..
}

void __ISR(_EXTERNAL_4_VECTOR, ipl3) _External4Handler(void)
{
    // clear the interrupt flag
    mINT4ClearIntFlag()();
    // .. things to do ..
    mPORTAToggleBits()
}

void __ISR(_EXTERNAL_3_VECTOR, ipl3) _External3Handler(void)
{
    // clear the interrupt flag
    mINT3ClearIntFlag()();
    // .. things to do ..
}

void __ISR(_EXTERNAL_2_VECTOR, ipl3) _External2Handler(void)
{
    // clear the interrupt flag
    mINT2ClearIntFlag()();
    // .. things to do ..
}

void __ISR(_EXTERNAL_1_VECTOR, ipl3) _External1Handler(void)
{
    // clear the interrupt flag
    mINT1ClearIntFlag()();
    // .. things to do ..
}
