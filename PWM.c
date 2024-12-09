#include "PWM.h"
#include <LPC17xx.h>
#include <PIN_LPC17xx.h>

void initPWM(){
	// Table 82.
	PIN_Configure(1, 18, 0b10, 0, 0);
//	LPC_PINCON->PINSEL3 = (1<<5);
	// Table 40.
	LPC_PWM1->PCR = PWM_SINGLE_EDGE;
	// TC increments, every 1 mili second
	LPC_PWM1->PR 	= 25000000/1000 - 1; // PR = (25MHz * 1ms) - 1 = (PCLK * PWM_res) - 1
	LPC_PWM1->MR0 = 20000; 	// setting period to 20000us = 20ms 
	LPC_PWM1->MR1 = 1250;		// setting pulse duration(width) to 1ms - setting PWM frequency to 1kHz
	
	LPC_PWM1->MCR = (1<<1);	// Reset PWM TC on PWM1MR0 match
	LPC_PWM1->LER = (1<<0) | (1<<1); // update MR0 and MR1, without it changes wont be applied
	LPC_PWM1->PCR = (1<<9);	// enable PWM1 output
	
	LPC_PWM1->TCR = (1<<1); //Reset PWM Timer Counter & Prescale Counter
	LPC_PWM1->TCR = (1<<0) | (1<<3);	// enable PWM Timer Counter and PWM Prescale Counter 
}

void changePulseWidth(unsigned int pulseWidth_us){
	LPC_PWM1->MR1 = pulseWidth_us;	// update pulse duration(width)
	LPC_PWM1->LER = (1<<1);			// apply change
}