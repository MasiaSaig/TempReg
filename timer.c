#include "timer.h"
#include <LPC17xx.h>

void initTimer(void){
	// Timer increments, every milisecond
	LPC_TIM0->CTCR = 0x0;
	LPC_TIM0->PR = 25000-1;		// TC increments every milisecond
														// [25MHz / s] = 1000 => [25MHz / 1000] = 25000Hz = s
	
	LPC_TIM0->TCR = (1<<1);		// reset Timer Counter and Prescale Counter
}

void resetTimer(void){
	LPC_TIM0->TCR = (1<<1);		// reset counter
	LPC_TIM0->TCR = (1<<0);		// start counting
}

void delay(unsigned int time_ms){
//	LPC_TIM0->TCR = (1<<1);		// reset counter
//	LPC_TIM0->TCR = (1<<0);		// start counting
	const uint32_t timeAfterDelay = LPC_TIM0->TC + time_ms;
	while(LPC_TIM0->TC < timeAfterDelay) {}
//	LPC_TIM0->TCR = (1<<1);		// reset counter
}

uint32_t timestamp(void){
	return LPC_TIM0->TC;
}