#include "SysTick.h"
#include "tm4c123gh6pm.h"

// Used by the interrupt hnadler
volatile uint32_t g_handler_calls;

// Initialize SysTick 
void SysTick_Init(void){
    NVIC_ST_CTRL_R = 0x05;  
}

// Initialize SysTick every 20 ms (assuming 16 MHz)
void SysTick_Init_Interrupts(void){
    g_handler_calls = 0;
    NVIC_ST_RELOAD_R  = 319999;  // 16 MHz/50 Hz - 1
    NVIC_ST_CURRENT_R = 0;       // clear
    NVIC_ST_CTRL_R    = 0x07;    
}

// 1 ms delay at 16 MHz
void SysTick_Delay1ms_16MHz(void){
    NVIC_ST_RELOAD_R  = 15999;   // 16 MHz / 1 kHz - 1
    NVIC_ST_CURRENT_R = 0;
    while((NVIC_ST_CTRL_R & 0x00010000) == 0){}
}

// 2 s delay at 16 MHz
void SysTick_Delay2s_16MHz(void){
		int i;
    for(i=0; i<2000; i++){
        SysTick_Delay1ms_16MHz();
    }
}

// 1 ms delay at 50 MHz
void SysTick_Delay1ms_50MHz(void){
    NVIC_ST_RELOAD_R  = 49999;   // 50 MHz / 1 kHz - 1
    NVIC_ST_CURRENT_R = 0;
    while((NVIC_ST_CTRL_R & 0x00010000) == 0){}
}

// 2 s delay at 50 MHz
void SysTick_Delay2s_50MHz(void){
		int i;
    for(i=0; i<2000; i++){
        SysTick_Delay1ms_50MHz();
    }
}

// Optional interrupt handler
void SysTick_Handler(void){
    g_handler_calls++;
    if((g_handler_calls % 50) == 0){
        GPIO_PORTF_DATA_R ^= 0x04;  // PF2 toggle
    }
    if((g_handler_calls % 100) == 0){
        GPIO_PORTF_DATA_R ^= 0x08;  // PF3 toggle
        g_handler_calls = 0;
    }
}
