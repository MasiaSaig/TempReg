#ifndef _REGULATOR_TIMER_H_
#define _REGULATOR_TIMER_H_

#include <LPC17xx.h>

void initTimer();
void resetTimer();

void delay(unsigned int time_ms);
uint32_t timestamp();

#endif