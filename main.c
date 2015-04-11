/* 
 * File:   main.c
 * Author: mtleising
 *
 * Created on March 25, 2015, 11:16 AM
 */

#include <p32xxxx.h>
#include <sys/attribs.h>
#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "w5200.h"

// Configuration Bit settings
// SYSCLK = 48 MHz (8MHz Crystal / FPLLIDIV * FPLLMUL / FPLLODIV)
// PBCLK = 48 MHz (SYSCLK / FPBDIV)
// Primary Osc w/PLL (XT+,HS+,EC+PLL)
// WDT OFF
// Other options are don't care
// 48MHz (Overclocked..)
// #pragma config FPLLMUL = MUL_24, FPLLIDIV = DIV_2, FPLLODIV = DIV_2, FWDTEN = OFF
// 40MHz
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_2, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_1

#define PPS_UNLOCK CFGCONbits.IOLOCK = 0;
#define PPS_LOCK CFGCONbits.IOLOCK = 1;

// Function prototypes
int main(void);
void sysyem_initialize(void);

int main(void)
{
    // Do I need to do this?
    // SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);

    sysyem_initialize();

    while(1)
    {
        //w5200_update();
    }

    return 1;
}

void sysyem_initialize(void)
{
    PPS_UNLOCK;
    // General Config
    // May not need this...
    //SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);

    // Ports
    // Set all digital
    ANSELA = 0x0000;
    ANSELB = 0x0000;
    ANSELC = 0x0000;

    // Config IO
    TRISA = PORTA_CONFIG;
    TRISB = PORTB_CONFIG;
    TRISC = PORTC_CONFIG;
    
    // Set all pins to low
    LATA  = 0x0000;
    LATB  = 0x0000;
    LATC  = 0x0000;

    // STATUS: Red for not ready
    LED1_G_OFF;
    LED2_G_OFF;
    LED1_R_ON;
    LED2_R_ON;

    // SPI
    spi_init();
    
    // INT PPS

    INT1R = ADC_OFLW_INT;
    INT2R = ADC_FLT_INT;
    INT3R = ADC_DATA_READY;
    INT4R = ETH_INT;

    PPS_LOCK;

    // Interrupt config
    
    // INT1 ? External Interrupt 1 8 7 IFS0<8> IEC0<8> IPC1<28:26> IPC1<25:24>
    // INT2 ? External Interrupt 2 13 11 IFS0<13> IEC0<13> IPC2<28:26> IPC2<25:24>
    // INT3 ? External Interrupt 3 18 15 IFS0<18> IEC0<18> IPC3<28:26> IPC3<25:24>
    // INT4 ? External Interrupt 4 23 19 IFS0<23> IEC0<23> IPC4<28:26> IPC4<25:24>    
    INTCON = EX_CON; // Edge trigger stuff here...    
    // Step 1: Disable Interrupts (Just kill em all Bow Bow Brrrrap)
    // Note: Don't know if I should actually kill them all...
    IEC0CLR = 0xFFFFFFFF;
    IEC1CLR = 0xFFFFFFFF;

    // Step 2: Set Priorities (PIC favorites.. hehe)
    // Once again, clear all priorities...
    IPC0CLR = 0xFFFFFFFF;
    IPC1CLR = 0xFFFFFFFF;
    IPC2CLR = 0xFFFFFFFF;
    IPC3CLR = 0xFFFFFFFF;
    IPC4CLR = 0xFFFFFFFF;
    IPC5CLR = 0xFFFFFFFF;
    IPC6CLR = 0xFFFFFFFF;
    IPC7CLR = 0xFFFFFFFF;
    IPC8CLR = 0xFFFFFFFF;
    IPC9CLR = 0xFFFFFFFF;
    IPC10CLR = 0xFFFFFFFF;
    
    // Set all priorities... H 7-0 Off
    IPC1SET = 0b00000100000000000000000000000000; // 1 OVERFLOW
    IPC2SET = 0b00001000000000000000000000000000; // 2 FAULT
    IPC3SET = 0b00011100000000000000000000000000; // 7 SAMPLE
    IPC4SET = 0b00001100000000000000000000000000; // 3 ETH
    
    // Step 3: Clear interrupt flags
    IFS0CLR = 0xFFFFFFFF;
    IFS1CLR = 0xFFFFFFFF;
    
    // STATUS: Green, pic and devices ready
    LED1_R_OFF;
    LED2_R_OFF;
    LED1_G_ON;
    LED2_G_ON;    
    
    // Init devices before turning on
    
    
    // Step 4: Enable Interrupts
    IEC0SET = 0b00000000100001000010000100000000;
}

void __ISR(_EXTERNAL_3_VECTOR, IPL7AUTO) _External3Handler(void) {
    // clear the interrupt flag
    //mINT3ClearIntFlag();
    // .. things to do ..
}

void __ISR(_EXTERNAL_4_VECTOR, IPL3AUTO) _External4Handler(void)
{
    // Disable Interrupts, indicate hard reset
    IEC0CLR = 0xFFFFFFFF;
    LED1_R_ON;
    LED2_R_ON;
    LED1_G_ON;
    LED2_G_ON;
}

void __ISR(_EXTERNAL_2_VECTOR, IPL2AUTO) _External2Handler(void)
{
    // Disable Interrupts, indicate hard reset
    IEC0CLR = 0xFFFFFFFF;
    LED1_R_ON;
    LED2_R_ON;
    LED1_G_OFF;
    LED2_G_ON;
}

void __ISR(_EXTERNAL_1_VECTOR, IPL1AUTO) _External1Handler(void)
{
    // Disable Interrupts, indicate hard reset
    IEC0CLR = 0xFFFFFFFF;
    LED1_R_ON;
    LED2_R_ON;
    LED1_G_ON;
    LED2_G_OFF;
}
