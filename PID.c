#include "PID.h"
#include "I2C_TMP2.h"

float Amplification_P = 0.1;
float Amplification_I = 0.01;
float Amplification_D = 0.1;

uint32_t deltaTime = 500;
int32_t sumTemperatureError = 0;

// TODO: calibrate, propert Amplification values
uint16_t calculatePID(void){
	uint16_t output = 0;
	uint16_t temperatureErrorPrev = temperatureError;
	temperatureError = setTemperature - currentTemperature;
	// TODO: set timings on retriving temperature data from sensor on constant itervals, 
	// if not, calculate time difference between retriving data
//	deltaTime = curTime - prevTime;
	sumTemperatureError += temperatureError*deltaTime;
	// boundary
	if(sumTemperatureError >  20000) { sumTemperatureError =  20000; }
	if(sumTemperatureError < -20000) { sumTemperatureError = -20000; }
	
	// P - Proportional
	output += Amplification_P * temperatureError;
	
	// I - Integral
	output += Amplification_I * sumTemperatureError;
	
	// D - Derivative
	output += Amplification_D * (temperatureError - temperatureErrorPrev) / deltaTime;
	
	return output;
}