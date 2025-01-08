#include <LPC17xx.h>
#include <PIN_LPC17xx.h>
#include "PWM.h"

void PWM_Init(uint32_t frequency){
    // Enable power for PWM1 - enabled by default after reset
    LPC_SC->PCONP |= (1 << SBIT_PC_PWM1);

    // Set PCLK for PWM1 to CCLK/4 - also default behavior
    LPC_SC->PCLKSEL0 &= ~(3 << SBIT_PCLK_PWM1); // reset values in bits 12:13
    LPC_SC->PCLKSEL0 |= (0 << SBIT_PCLK_PWM1);  // set them to 00 - default

    // Configure P1.18 as PWM1.1 (Function 10) 
	// P2.0 breaks LCD, better use pin 1.18
    PIN_Configure(1, 18, 0b10, 0, 0); // alternative using PIN lib

    // Set PWM1 to single edge mode - redundant, bit 0:1 unused 
    LPC_PWM1->PCR = SINGLE_EDGE_PWM;

    // Set prescaler value - TC increment every 1us
    LPC_PWM1->PR = PRESCALER;

    // number of cycles needed to perform changes in duty cycle with given frequency
    LPC_PWM1->MR0 = PCLK / (PRESCALER + 1) / frequency;

    // 50% duty cycle as default
    LPC_PWM1->MR1 = LPC_PWM1->MR0 / 2;

    // Reset PWM TC (counter) on PWM1MR0 match ( no interrupts )
    LPC_PWM1->MCR = (1 << 1);

    // reset PWM Timer Counter & Prescale Counter
    LPC_PWM1->TCR = (1 << 1);
    // enable PWM Timer Counter and PWM Prescale Counter
    LPC_PWM1->TCR = (1 << 0) | (1 << 3);

    LPC_PWM1->LER = (1 << 1) | (1 << 0); // update values in MR0 and MR1
    LPC_PWM1->PCR = (1 << 9);            // enable PWM1 output
}

void PWM_SetDutyCycle(uint8_t dutyCycle){
    if (dutyCycle > 100)      // No more than 100%
			dutyCycle = 100;
		else if(dutyCycle < 0)    // No less than 0%
			dutyCycle = 0;
    LPC_PWM1->MR1 = (LPC_PWM1->MR0 * dutyCycle) / 100;
    LPC_PWM1->LER = (1 << 1); // Load MR1 value
}