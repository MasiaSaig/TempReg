#ifndef _REGULATOR_GLOBALS_H_
#define _REGULATOR_GLOBALS_H_

#include <LPC17xx.h>

// temperature set by user, which regulator should aim to achive
extern uint16_t setTemperature;
// current temperature, captured by sensor
extern uint16_t currentTemperature;
// type of heater regulator: either proportional or 2-positional 
extern uint8_t proportionalControl; // cant make 1-bit variable, so this must do
extern uint16_t temperatureDifference;

#endif