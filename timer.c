#include "timer.h"
#include <LPC17xx.h>

void initTimer(){
	// Timer increments, every milisecond
	LPC_TIM0->PR = 25000-1;		// TC increments every milisecond
														// [25MHz / s] = 1000 => [25MHz / 1000] = 25000Hz = s
	
	LPC_TIM0->TCR = (1<<1);		// reset Timer Counter and Prescale Counter
}

void resetTimer(){
	LPC_TIM0->TCR = (1<<1);		// reset counter
	LPC_TIM0->TCR = (1<<0);		// start counting
}

void delay(unsigned int time_ms){
//	LPC_TIM0->TCR = (1<<1);		// reset counter
//	LPC_TIM0->TCR = (1<<0);		// start counting
	const unsigned int timeAfterDelay = LPC_TIM0->TCR + time_ms;
	while(LPC_TIM0->TCR < timeAfterDelay) {}
//	LPC_TIM0->TCR = (1<<1);		// reset counter
}

uint16_t timestamp(){
	return LPC_TIM0->TCR;
}