#include "timer.h"
#include <LPC17xx.h>

timerFlags timerStatus = {0};

void initTimer(void){
	/* Table 425 */
	// Timer increments, every milisecond
	LPC_TIM0->CTCR = 0x0;
	LPC_TIM0->PR = 25000-1;		// TC increments every milisecond
														// [25MHz / s] = 1000 => [25MHz / 1000] = 25000Hz = s
	LPC_TIM0->MCR = (1<<0);							// if TC == MR0, generate interrupt
	LPC_TIM0->MCR |= (1<<3) | (1<<4);		// if TC == MR1, generate interrupt and reset timer
	LPC_TIM0->MR0 = 1;
	LPC_TIM0->MR1 = 1000;
	LPC_TIM0->TCR = (1<<0);		// enable TC and PC counting
}

/*
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
*/

void TIMER0_IRQHandler(){
	timerStatus.bits = 0;			// reset all flags to zero
	timerStatus.f1ms = 1;						// set flag every milisecond
	if((LPC_TIM0->TC & 5)   == 0)
		timerStatus.f5ms = 1;
	if((LPC_TIM0->TC & 50)  == 0)		// set flag every 50 miliseconds
		timerStatus.f50ms = 1;
	if((LPC_TIM0->TC & 500) == 0)	// set flag every 500 miliseconds
		timerStatus.f500ms = 1;
	
	LPC_TIM0->IR |= 1;	// reset MR0 interrupt
	if((LPC_TIM0->IR & 0b10) == 0b10){	// if MR1 interrupt
		timerStatus.f1s = 1;
		LPC_TIM0->IR |= (1 << 1);	// reset MR1 interrupt
	}
}