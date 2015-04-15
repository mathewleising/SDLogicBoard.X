/* 
 * File:   main.c
 * Author: mtleising
 *
 * Created on March 25, 2015, 11:16 AM
 */

#include <plib.h>
#include <xc.h>
#include <sys/attribs.h>
#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "w5200.h"
#include "max110.h"

// DEVCFG3
// USERID = No Setting
#pragma config PMDL1WAY = ON            // Peripheral Module Disable Configuration (Allow only one reconfiguration)
#pragma config IOL1WAY = ON             // Peripheral Pin Select Configuration (Allow only one reconfiguration)
#pragma config FUSBIDIO = ON            // USB USID Selection (Controlled by the USB Module)
#pragma config FVBUSONIO = ON           // USB VBUS ON Selection (Controlled by USB Module)

// DEVCFG2
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
#pragma config FPLLMUL = MUL_20         // PLL Multiplier (20x Multiplier), MUL_24 for 48
#pragma config UPLLIDIV = DIV_12        // USB PLL Input Divider (12x Divider)
#pragma config UPLLEN = OFF             // USB PLL Enable (Disabled and Bypassed)
#pragma config FPLLODIV = DIV_2         // System PLL Output Clock Divider (PLL Divide by 2)

// DEVCFG1
#pragma config FNOSC = PRIPLL           // Oscillator Selection Bits (Primary Osc w/PLL (XT+,HS+,EC+PLL))
#pragma config FSOSCEN = ON             // Secondary Oscillator Enable (Enabled)
#pragma config IESO = ON                // Internal/External Switch Over (Enabled)
#pragma config POSCMOD = HS             // Primary Oscillator Configuration (HS osc mode)
#pragma config OSCIOFNC = OFF           // CLKO Output Signal Active on the OSCO Pin (Disabled)
#pragma config FPBDIV = DIV_1           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/1)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
#pragma config WDTPS = PS1048576        // Watchdog Timer Postscaler (1:1048576)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable (Watchdog Timer is in Non-Window Mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))
#pragma config FWDTWINSZ = WINSZ_25     // Watchdog Timer Window Size (Window Size is 25%)

// DEVCFG0
#pragma config JTAGEN = ON              // JTAG Enable (JTAG Port Enabled)
#pragma config ICESEL = ICS_PGx3        // ICE/ICD Comm Channel Select (Communicate on PGEC3/PGED3)
#pragma config PWP = OFF                // Program Flash Write Protect (Disable)
#pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
#pragma config CP = OFF                 // Code Protect (Protection Disabled)

#define PPS_UNLOCK CFGCONbits.IOLOCK = 0;
#define PPS_LOCK CFGCONbits.IOLOCK = 1;
#define EXT1_INT_MASK 0x00000100
#define EXT2_INT_MASK 0x00002000
#define EXT3_INT_MASK 0x00040000
#define EXT4_INT_MASK 0x00800000

// Function prototypes
int main(void);
void sysyem_initialize(void);
void die(void);
MAX110_d sensors;


int main(void)
{
    // Do I need to do this?
    // SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
    SYSTEMConfigPerformance(SYS_FREQ);


    sysyem_initialize();

    while(1)
    {
        if (w5200_buff(0x5555)<0)
            die();
        
        //delay_for_1000_nops();
        w5200_update();
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

    IFS0CLR = 0xFFFFFFFF;
    IFS1CLR = 0xFFFFFFFF;

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

    // Step 4: Enable Interrupts
    IEC0SET = 0b00000000100001000010000100000000;


    INTEnableSystemMultiVectoredInt();
    
    PPS_LOCK;
        // Init devices before turning on
    while (w5200_init()<0)
        ;
    max110_init();

    // STATUS: Green, pic and devices ready
    LED1_R_OFF;
    LED2_R_OFF;
    LED1_G_ON;
    LED2_G_ON;
}

void die(void)
{
    // Disable Interrupts, indicate hard reset
    IEC0CLR = 0xFFFFFFFF;
    LED1_R_ON;
    LED2_R_ON;
    LED1_G_OFF;
    LED2_G_ON;
}

void __ISR(_EXTERNAL_3_VECTOR, IPL7AUTO) _External3Handler(void) {
    // clear the interrupt flag
    IFS0CLR = EXT3_INT_MASK;


    get_data(&sensors);

    LED1_G_ON;
    LED2_G_OFF;
    LED1_R_OFF;
    LED2_R_ON;

}

void __ISR(_EXTERNAL_4_VECTOR, IPL3AUTO) _External4Handler(void)
{
    IFS0CLR = EXT4_INT_MASK;
    // Disable Interrupts, indicate hard reset
    die();
}

void __ISR(_EXTERNAL_2_VECTOR, IPL2AUTO) _External2Handler(void)
{
    IFS0CLR = EXT2_INT_MASK;
    // Disable Interrupts, indicate hard reset
    die();
}

void __ISR(_EXTERNAL_1_VECTOR, IPL1AUTO) _External1Handler(void)
{
    IFS0CLR = EXT1_INT_MASK;
    // Disable Interrupts, indicate hard reset
    die();
}
