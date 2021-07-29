#ifndef THREAD_RAM_H
#define THREAD_RAM_H
#include "Os.h"

typedef struct TCB_Struct{
    uint32_t *Lsp;
    struct TCB_Struct *next;
} TCB;

typedef void(*TaskType)(void);

#if(NUMBER_OF_THREAD > 0)
extern TCB *GddCurrentTCB;
extern TCB GddTCBArray[];
extern uint32_t GaaStack[][STACK_SIZE];
extern TaskType GaaTaskArray[];
#endif

#if(NUMBER_OF_EVENT_TASK > 0)
extern uint16_t GusEventPeriod[];
extern uint16_t GusEventTickOffset[];
extern TaskType GaaEventArray[];
#endif

#endif