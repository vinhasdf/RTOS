#include "Thread_Ram.h"

extern void Task1(void);
extern void Task2(void);
extern void Event1(void);
extern void Event2(void);

void Default_Task(void)
{
    Os_SusPend();
    while(1)
    {

    }
}

#if(NUMBER_OF_EVENT_TASK > 0)
uint16_t GusEventPeriod[NUMBER_OF_EVENT_TASK] = {
    1*OS_Tick_FreQuence,                            // 1 s
    (10*OS_Tick_FreQuence)/1000                     // 10ms
};

uint16_t GusEventTickOffset[NUMBER_OF_EVENT_TASK] = {
    0,
    1
};
TaskType GaaEventArray[NUMBER_OF_EVENT_TASK] = {
    Event1,
    Event2
};
#endif

#if(NUMBER_OF_THREAD > 0)
TCB *GddCurrentTCB;
TCB GddTCBArray[NUMBER_OF_THREAD];
uint32_t GaaStack[NUMBER_OF_THREAD][STACK_SIZE];
TaskType GaaTaskArray[NUMBER_OF_THREAD] = {
    Default_Task,
    Task1,
    Task2
};
#endif