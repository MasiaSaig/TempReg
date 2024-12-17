#ifndef _TEMPERATURE_REGULATOR_H_
#define _TEMPERATURE_REGULATOR_H_

#include <LPC17xx.h>

#define MAX_TEMPERATURE 65
#define MIN_TEMPERATURE 0

extern float Amplification_P;
extern float Amplification_I;
extern float Amplification_D;

extern const uint32_t deltaTime_us;
extern int32_t sumTemperatureError;
extern uint16_t setTemperature;				//!< temperature set by user, which regulator should aim to achive
extern int16_t temperatureError;
extern uint16_t heaterPower;

// represents type of heater regulator: either proportional or 2-positional 
extern uint8_t PIDControl; 						//!< cant make 1-bit variable, so this must do

//float calculatePID(uint32_t deltaTime);
void calculatePID(void);
void twoPositionalControl(void);

#endif