#ifndef _TEMPERATURE_REGULATOR_H_
#define _TEMPERATURE_REGULATOR_H_

#include <LPC17xx.h>

#define LIMIT 1000000

extern float Amplification_P;
extern float Amplification_I;
extern float Amplification_D;

extern const uint32_t deltaTime_us;
extern int32_t sumTemperatureError;
extern uint16_t setTemperature;	//!< temperature set by user, which regulator should aim to achive
extern uint16_t temperatureError;
// type of heater regulator: either proportional or 2-positional 
extern uint8_t proportionalControl; //!< cant make 1-bit variable, so this must do

//float calculatePID(uint32_t deltaTime);
uint16_t calculatePID(void);
void twoPositionalControl(void);


#endif