#include "Os.h"
#include "Thread_Ram.h"

void Os_SusPend(void)
{
  SCB->ICSR |= (1 << 26);         // Set pending Systick interrupt bit
}

void Sem_Up(uint8_t *LpSem)
{
  __disable_irq();
  *LpSem += 1;
  __enable_irq();
}

void Sem_Down(uint8_t *LpSem)
{
  while(1)
  {
    __disable_irq();
    if(*LpSem != 0)
    {
      break;
    }
    __enable_irq();
  }
  *LpSem -= 1;
  __enable_irq();
}

#if(NUMBER_OF_THREAD > 0)
void Init_Stack(uint8_t Lstacknum)
{
  uint32_t *LarrCurArray;
  LarrCurArray = GaaStack[Lstacknum];
  LarrCurArray[STACK_SIZE - 1] = 0x01000000; // Reset value of PSR, enable thumb state
  LarrCurArray[STACK_SIZE - 2] = (uint32_t)GaaTaskArray[Lstacknum];   // Hold pc counter
  LarrCurArray[STACK_SIZE - 3] = 0xFFFFFFFF; // LR register       // Default value after reset
  LarrCurArray[STACK_SIZE - 4] = 0x12121212; // R12 register , Random value
  LarrCurArray[STACK_SIZE - 5] = 0x03030303; // R3 register
  LarrCurArray[STACK_SIZE - 6] = 0x02020202; // R2
  LarrCurArray[STACK_SIZE - 7] = 0x01010101; // R1
  LarrCurArray[STACK_SIZE - 8] = 0x00100100; // R0
  LarrCurArray[STACK_SIZE - 9] = 0x11111111; // R11
  LarrCurArray[STACK_SIZE - 10] = 0x10101010; // R10
  LarrCurArray[STACK_SIZE - 11] = 0x09090909; // R9
  LarrCurArray[STACK_SIZE - 12] = 0x08080808; // R8
  LarrCurArray[STACK_SIZE - 13] = 0x07070707; // R7
  LarrCurArray[STACK_SIZE - 14] = 0x06060606; // R6
  LarrCurArray[STACK_SIZE - 15] = 0x05050505; // R5
  LarrCurArray[STACK_SIZE - 16] = 0x04040404; // R4
}
#endif

void OS_Init(void)
{
  #if(NUMBER_OF_THREAD > 0)
  uint8_t i;

  for(i = 0; i < NUMBER_OF_THREAD; i++)
  {
    Init_Stack(i);
    GddTCBArray[i].next = &GddTCBArray[(i + 1)%NUMBER_OF_THREAD];
    GddTCBArray[i].Lsp = &(GaaStack[i][STACK_SIZE - 16]);
  }

  GddCurrentTCB = &GddTCBArray[0];
  #endif
}

void OS_Launch(void)
{
  #if(NUMBER_OF_THREAD > 0)
  asm("cpsid i");
  SysTick_Config(SystemCoreClock/OS_Tick_FreQuence);    // 2ms run
  asm("LDR r0, =GddCurrentTCB");          // Load r0 with address of GddCurrentTCB
  asm("LDR r1, [r0]");                    // Load r1 with value of GddCurrentTCB, = address of Spt
  asm("LDR sp, [r1]");                    // load sp with  spt
  asm("POP {r4-r11}");                    // load register to normal start
  asm("POP {r0-r3}");                     // load register to normal start
  asm("POP {r12}");
  asm("ADD sp, sp, #4");                  // Discart LR
  asm("POP {LR}");                        // Store pc to LR
  asm("ADD sp, sp, #4");                  // Discart PSR
  asm("cpsie i");
  asm("BX LR");                           // Jump to Task
  #else
  SysTick_Config(SystemCoreClock/OS_Tick_FreQuence);    // 2ms run
  #endif
}

void Change_CurrentTCB(void)
{
  // Handler event tasks
  #if(NUMBER_OF_EVENT_TASK > 0)
  static uint16_t LOscount = 0;
  uint8_t i;
  LOscount = (LOscount + 1)%OS_Tick_Max_Count;
  
  for(i = 0; i < NUMBER_OF_EVENT_TASK; i++)
  {
    if((LOscount % GusEventPeriod[i]) == GusEventTickOffset[i])
    {
      GaaEventArray[i]();
      break;
    }
  }
  #endif
  // end handler event task
  
  #if(NUMBER_OF_THREAD > 0)
  GddCurrentTCB = GddCurrentTCB->next;
  #endif
}

void SysTick_Handler(void)
{
  // Perform Task switch
  #if(NUMBER_OF_THREAD > 0)
  asm("cpsid i");         // diable interrupt
  //asm("POP {r7}");        // Need enable optimization code for avoid this step, C functioncs auto Push r7 on start of function
  // store ole r4-r11 to current spt, sp is current point to top os task stack
  asm("PUSH {r4-r11}");               /* save value r4-r11, this register will reload to r4-r11 next call of systick
                                      */

  asm("LDR r0, =GddCurrentTCB");      // Load address of GddCurrentTCB
  asm("LDR r1, [r0]");                // value of TCB, address of spt
  asm("STR sp, [r1]");                // store old stack pointer to value of spt

  asm("PUSH {r0, LR}");
  asm("bl Change_CurrentTCB");
  asm("pop {r0, LR}");

  asm("LDR r1, [r0]");                // load r1 with value of next ptr, = address of next spt
  asm("LDR SP, [r1]");                // load sp with new value of spt of new TCB

  asm("POP {r4-r11}");                // reload value from r4 - r11
  asm("cpsie i");                     // enable interrupt
  asm("bx lr");
  #else
  Change_CurrentTCB();
  #endif
}