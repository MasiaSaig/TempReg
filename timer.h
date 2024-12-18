#ifndef _REGULATOR_TIMER_H_
#define _REGULATOR_TIMER_H_

#include <LPC17xx.h>

typedef union{
	struct {
		uint8_t f1ms		: 1;
		uint8_t f5ms		: 1;
		uint8_t f50ms		: 1;
		uint8_t f250ms	: 1;
		uint8_t f500ms	: 1;
		uint8_t f1s			: 1;
		uint8_t 				: 2; 	//!< unused
	};
	uint8_t bits;
} timerFlags;

extern timerFlags timerStatus;  //!< 8-bit union/struct containing time intervals flags of timer.

/*! Initialization of timer, TIM0 
 *
 * Initializes timer TIM0 to generate interrupt every 1 milisecond. 
 * Within interrupt handling function, it changes according timerStatus flags.
 *
 * @return void
 */
void initTimer(void);

#endif