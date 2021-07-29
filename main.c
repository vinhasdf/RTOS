#include "stm32f10x.h"
#include "Thread_Ram.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

uint8_t Semmaphore1;
uint8_t Semmaphore2;
uint8_t ShareGPIOA = 1;

void Event1(void)
{
    if(Semmaphore1 == 0)
    {
        Sem_Up(&Semmaphore1);
    }
    else
    {
        // Error
    }
}

void Event2(void)
{
    static uint8_t LucDebugvar = 0;
    uint8_t LucpinState;
    LucpinState = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2);
    if(LucpinState == Bit_RESET)
    {
        if(LucDebugvar == 2)
        {
            Sem_Up(&Semmaphore2);
        }
        // Debound button and not send signal when button is held
        LucDebugvar++;
    }
    else
    {
        LucDebugvar = 0;
    }
}

void Task1(void)
{
    //asm("POP {r7}");   C code auto push old r7 to stack in start bracket of function, only pop this in C perform asm funcs to avoid error
    while(1)
    {
        Sem_Down(&Semmaphore1);
        Sem_Down(&ShareGPIOA);
        GPIO_WriteBit(GPIOA, GPIO_Pin_0, 
            (BitAction)!GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_0));
        Sem_Up(&ShareGPIOA);
    }
}

void Task2(void)
{
    //asm("POP {r7}");  // Addition step to retake r7 from C code when jump to start address of C functions, real size of stack is (STACK_SIZE - 1)*4
    while(1)
    {
        Sem_Down(&Semmaphore2);
        Sem_Down(&ShareGPIOA);
        GPIO_WriteBit(GPIOA, GPIO_Pin_1, 
            (BitAction)!GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_1));
        Sem_Up(&ShareGPIOA);
    }
}

int main(void)
{
    GPIO_InitTypeDef LddLcd_gpio;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // Two output pin
    LddLcd_gpio.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_0;
    LddLcd_gpio.GPIO_Speed = GPIO_Speed_50MHz;
    LddLcd_gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &LddLcd_gpio);

    // Config button pin
    LddLcd_gpio.GPIO_Pin = GPIO_Pin_2;
    LddLcd_gpio.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &LddLcd_gpio);
    
    OS_Init();
    OS_Launch();

    while(1)
    {
    }
    return 0;
}