#include "delay.h"

void delay_for_1000_nops_x (uint16_t n)
{
    while (n --)
    {
        int i;

        for (i = 0; i < 1000; i++)
            asm volatile ("nop");
    }
}

void delay_for_1000_nops ()
{
    delay_for_1000_nops_x (1);
}

// The following function is adopted from
// Digital Design and Computer Architecture
// by David Harris & Sarah Harris

void delay_micros (uint16_t n)
{
    const uint16_t function_overhead = 6;

    while (n > 1000)  // To avoid timer overflow
    {
        delay_micros (1000);
        n -= 1000;
    }

    if (n > function_overhead)
    {
        T2CONCLR = 0xFFFFFFFF;
        IFS0CLR = 0x00000200;
        TMR2 = 0;
        PR2 = (n - function_overhead) * (PBCLK_FREQUENCY / 1000) / 1000;
        T2CONSET = 0x00008000;

        while (!IFS0bits.T2IF)  // Wait until overflow flag is set
            ;
    }
}

void delay_millis (uint16_t n)
{
    while (n --)
        delay_micros (1000);
}

void delay_seconds (uint16_t n)
{
    while (n --)
        delay_millis (1000);
}

