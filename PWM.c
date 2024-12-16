#include "PWM.h"
#include <LPC17xx.h>
#include <PIN_LPC17xx.h>

void initPWM(void){
	/* Table 82. */
	PIN_Configure(1, 18, 0b10, 0, 0);
	/* Table 40 */
	LPC_PWM1->PCR = PWM_SINGLE_EDGE;
	// TC increments, every 1 mili second
	LPC_PWM1->PR  = 25000000/1000 - 1; // PR = (25MHz * 1ms) - 1 = (PCLK * PWM_res) - 1
	LPC_PWM1->MR0 = 1000000; 	// setting period to 1000000us = 1000ms = 1s 
	LPC_PWM1->MR1 =  500000;	// setting pulse duration(width) to 0.5s
	
	LPC_PWM1->MCR = (1<<1);						// reset PWM TC on PWM1MR0 match
	LPC_PWM1->LER = (1<<0) | (1<<1);	// update MR0 and MR1, without it changes wont be applied
	LPC_PWM1->PCR = (1<<9);						// enable PWM1 output
	
	LPC_PWM1->TCR = (1<<1); 					// reset PWM Timer Counter & Prescale Counter
	LPC_PWM1->TCR = (1<<0) | (1<<3);	// enable PWM Timer Counter and PWM Prescale Counter 
}

void changePulseWidth(unsigned int pulseWidth_ms){
	LPC_PWM1->MR1 = pulseWidth_ms*1000;	// update pulse duration(width)
	LPC_PWM1->LER = (1<<1);			// apply change
}