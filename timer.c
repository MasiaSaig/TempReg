#include "timer.h"
#include <LPC17xx.h>

timerFlags timerStatus = {0};

void initTimer(void){
	/* Table 425 */
	// Timer increments, every milisecond
	LPC_TIM0->CTCR = 0x0;
	LPC_TIM0->PR = 25000-1;		// TC increments every milisecond
														// [25MHz / s] = 1000 => [25MHz / 1000] = 25000Hz = s
	LPC_TIM0->MCR = (1<<0) | (1<<1);		// if TC == MR0, generate interrupt
	LPC_TIM0->MR0 = 1;
	LPC_TIM0->TCR = (1<<0);		// enable TC and PC counting
	
	NVIC_EnableIRQ(TIMER0_IRQn);
}

uint8_t tick = 0;
void TIMER0_IRQHandler(void){
	// TODO: check if reseting all flags like that, will work (if statements in main loop might not run)
//	timerStatus.bits = 0;			// reset all flags to zero
	++tick;
	timerStatus.f1ms = 1;       // set flag every milisecond
	if((tick % 5) == 0)         // set flag every 5 miliseconds
		timerStatus.f5ms = 1;
	if((tick % 50) == 0)		    // set flag every 50 miliseconds
		timerStatus.f50ms = 1;
	if((tick % 250) == 0)       // set flag every 250 miliseconds
		timerStatus.f250ms = 1;
	if((tick % 500) == 0)	      // set flag every 500 miliseconds
		timerStatus.f500ms = 1;
	if((tick % 1000) == 0){     // set flag every second and reset tick
		timerStatus.f1s = 1;
		tick = 0;
	}
  LPC_TIM0->IR |= 1;	// reset MR0 interrupt
}
