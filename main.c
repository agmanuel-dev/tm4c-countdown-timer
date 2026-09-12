// main.c for final project
// Single-digit 7-segment countdown with button start and PB4 indicator LED (with fade-in and 5s hold)

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "SysTick.h"    // provides SysTick_Init() & SysTick_Delay1ms_16MHz()

// 7-segment bit patterns for 0-9
static const uint8_t segment_map[10] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9
};


// Pin definitions
#define BUTTON_PIN   (1U<<4)   // PF4, external 10 kO pull-down + pushbutton to +3.3 V
#define IND_LED_PIN  (1U<<4)   // PB4, indicator LED


// Initialize Port F: PF1-PF3 
static void PortF_Init(void){
    SYSCTL_RCGCGPIO_R |= (1U<<5);               // enable clock to GPIO
    while((SYSCTL_PRGPIO_R & (1U<<5)) == 0){}   // Here I wait

    // PF1, PF2, PF3 = outputs for segments A, B, C
    GPIO_PORTF_DIR_R  |= (1U<<1)|(1U<<2)|(1U<<3);
    // PF4 = input for the button
    GPIO_PORTF_DIR_R  &= ~BUTTON_PIN;

    // digital enable for PF1-PF4
    GPIO_PORTF_DEN_R  |= (1U<<1)|(1U<<2)|(1U<<3)|BUTTON_PIN;
}


// Initialize Port B: PB0-PB3 = segments D-G outputs, PB4 = for LED output
static void PortB_Init(void){
    SYSCTL_RCGCGPIO_R |= (1U<<1);               // enable clock to GPIO
    while((SYSCTL_PRGPIO_R & (1U<<1)) == 0){}   // Wait again

    // PB0-PB3 = outputs for segments D-G for the digital display
    // PB4    = output for LED
    GPIO_PORTB_DIR_R  |= 0x1F;   // bits 0-4
    GPIO_PORTB_DEN_R  |= 0x1F;   // digital enable for thwe bits
}


// Display a single digit (0-9) on the 7-segment. 
static void displayDigitStatic(int value){
    uint8_t seg = segment_map[value];

    // segments A-C on PF1-PF3
    GPIO_PORTF_DATA_R = (GPIO_PORTF_DATA_R & ~0x0E) | ((seg & 0x07) << 1);
    // segments D-G on PB0-PB3 
    GPIO_PORTB_DATA_R = (GPIO_PORTB_DATA_R & ~0x0F) | ((seg >> 3) & 0x0F);
}


// Fade-in LED on PB4 over 2 seconds using PWM
static void FadeIn_LED(void) {
    const uint32_t steps = 100;      //cycle steps from 0% to 100%
    const uint32_t period_ms = 20;   // 20 ms PWM period
		uint32_t s;
    for (s = 0; s < steps; s++) {
        uint32_t on_ms  = (period_ms * s) / steps;
        uint32_t off_ms = period_ms - on_ms;
        // PWM ON phase
        GPIO_PORTB_DATA_R |=  IND_LED_PIN;
				uint32_t t;
        for (t = 0; t < on_ms; t++) {
            SysTick_Delay1ms_16MHz();
        }
        // PWM OFF phase
        GPIO_PORTB_DATA_R &= ~IND_LED_PIN;
        for (t = 0; t < off_ms; t++) {
            SysTick_Delay1ms_16MHz();
        }
    }
}



int main(void){
    uint32_t sec, ms;

    PortF_Init();        // segments A-C, button input
    PortB_Init();        // segments D-G, indicator LED
    SysTick_Init();      // for the 1 ms delay

    // Here i want to make sure
	// indicator LED (PB4) is off initially
    GPIO_PORTB_DATA_R &= ~IND_LED_PIN;

    while(1){
        //Wait for button press 
        if (GPIO_PORTF_DATA_R & BUTTON_PIN) {

            for(ms = 0; ms < 20; ms++) SysTick_Delay1ms_16MHz();
            if (!(GPIO_PORTF_DATA_R & BUTTON_PIN)) continue;

            // Countdown 9, one second per digit
            for(sec = 9; ; sec--){
                displayDigitStatic((int)sec);
                for(ms = 0; ms < 1000; ms++){
                    SysTick_Delay1ms_16MHz();
                }
                if (sec == 0) break;
            }

            // Fade in the LED
            FadeIn_LED();

            GPIO_PORTB_DATA_R |= IND_LED_PIN;
            // Hold LED on for 5 seconds
            for(ms = 0; ms < 5000; ms++){
                SysTick_Delay1ms_16MHz();
            }
            // Turn LED off after hold
            GPIO_PORTB_DATA_R &= ~IND_LED_PIN;

            // Wait for button release
            while (GPIO_PORTF_DATA_R & BUTTON_PIN) {}
        }
    }
}