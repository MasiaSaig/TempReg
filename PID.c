#include "PID.h"
#include "globals.h"

float Amplification_P = 0.1;
float Amplification_I = 0.01;
float Amplification_D = 0.1;

void calculatePID(){
	float output = 0;
	temperatureError = setTemperature - currentTemperature;
	// TODO: set timings on retriving temperature data from sensor on constant itervals, 
	// if not, calculate time difference between retriving data
//	deltaTime = curTime - prevTime;
//	sumTemperatureError += temperatureError*deltaTime;
//	if(sumTemperatureError >  20000) { sumTemperatureError =  20000; }
//	if(sumTemperatureError < -20000) { sumTemperatureError = -20000; }
	
	// P - Proportional
	output += Amplification_P * temperatureError;
	
	// I - Integral
//	output += Amplification_I * sumTemperatureError;
	
	// D - Derivative
//	output += Amplification_D * (temperatureError - temperatureErrorPrev) / deltaTime;
}