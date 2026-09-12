#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

// Basic SysTick setup 
void SysTick_Init(void);

// SysTick every 20 ms 
void SysTick_Init_Interrupts(void);

// Blocking delays:
void SysTick_Delay1ms_16MHz(void);
void SysTick_Delay1ms_50MHz(void);
void SysTick_Delay2s_16MHz(void);
void SysTick_Delay2s_50MHz(void);

#endif // SYSTICK_H
