#include "timer.h"
#include <LPC17xx.h>

void initTimer(){
	LPC_TIM0->PR = 25000-1;		// TC increments every milisecond
														// [25MHz / s] = 1000 => [25MHz / 1000] = 25000Hz = s
	
	LPC_TIM0->TCR = (1<<1);		// reset Timer Counter and Prescale Counter
}

void delay(unsigned int time_ms){
	LPC_TIM0->TCR = (1<<1);		// reset counter
	LPC_TIM0->TCR = (1<<0);		// start counting
	while(LPC_TIM0->TCR < time_ms) {}
	LPC_TIM0->TCR = (1<<1);		// reset counter
}