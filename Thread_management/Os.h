#ifndef _OS_H_
#define _OS_H_

#include "stm32f10x.h"

#define STACK_SIZE              128U
#define NUMBER_OF_THREAD        3U
#define NUMBER_OF_EVENT_TASK    2U

#define OS_Tick_FreQuence   1000U
#define OS_Tick_Max_Count   1000U           // For event thead calculate


void OS_Init(void);
void OS_Launch(void);
void Sem_Up(uint8_t *LpSem);
void Sem_Down(uint8_t *LpSem);
void Os_SusPend(void);

#endif