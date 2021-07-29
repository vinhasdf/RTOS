#include "delay.h"

void delay_ms(uint16_t Ltime)
{
    while(Ltime--)
    {
        delay_us(1000);
    }
}

void delay_us(uint16_t Ltime)
{
    while(Ltime--)
    {
        asm("NOP");    asm("NOP");    asm("NOP");    asm("NOP");    asm("NOP");
        asm("NOP");    asm("NOP");    asm("NOP");    asm("NOP");    asm("NOP");
        asm("NOP");    asm("NOP");    asm("NOP");    asm("NOP");    asm("NOP");
        asm("NOP");    asm("NOP");    asm("NOP");    asm("NOP");    asm("NOP");
        asm("NOP");    asm("NOP");    asm("NOP");    asm("NOP");    asm("NOP");
        asm("NOP");    asm("NOP");    asm("NOP");    asm("NOP");    asm("NOP");
        asm("NOP");    asm("NOP");    asm("NOP");    asm("NOP");    asm("NOP");
        asm("NOP");    asm("NOP");    asm("NOP");    asm("NOP");    asm("NOP");
        asm("NOP");    asm("NOP");    asm("NOP");    asm("NOP");    asm("NOP");
        asm("NOP");    asm("NOP");    asm("NOP");    asm("NOP");    asm("NOP");
        asm("NOP");    asm("NOP");    asm("NOP");    asm("NOP");    asm("NOP");
        asm("NOP");    asm("NOP");    asm("NOP");    asm("NOP");    asm("NOP");
        asm("NOP");    asm("NOP");    asm("NOP");    asm("NOP");    asm("NOP");
        asm("NOP");    asm("NOP");    asm("NOP");    asm("NOP");    asm("NOP");
        asm("NOP");    asm("NOP");
    }
}