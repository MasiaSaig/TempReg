#ifndef _PID_REGULATOR_H_
#define _PID_REGULATOR_H_

#include <LPC17xx.h>

extern float Amplification_P;
extern float Amplification_I;
extern float Amplification_D;

void calculatePID();

#endif