#include "tempRegulation.h"
#include "I2C_TMP2.h"

// TODO: tune amplifications
float Amplification_P = 1; //0.1;
float Amplification_I = 0; //0.01;
float Amplification_D = 0; //0.1;

const uint32_t deltaTime_us = 50000; // == 50 ms
int32_t sumTemperatureError = 0;

// TODO: calibrate, propert Amplification values
uint16_t calculatePID(void){
	uint16_t output = 0;
	uint16_t temperatureErrorPrev = temperatureError;
	temperatureError = setTemperature - currentTemperature;
	// TODO: set timings on retriving temperature data from sensor on constant itervals, 
	// if not, calculate time difference between retriving data
//	deltaTime = curTime - prevTime;
	sumTemperatureError += temperatureError*deltaTime_us;
	// boundary
	if(sumTemperatureError >  LIMIT) { sumTemperatureError =  LIMIT; }
	if(sumTemperatureError < -LIMIT) { sumTemperatureError = -LIMIT; }
	
	// P - Proportional
	output += Amplification_P * temperatureError;
	
	// I - Integral
	output += Amplification_I * sumTemperatureError;
	
	// D - Derivative
	output += Amplification_D * (temperatureError - temperatureErrorPrev) / deltaTime_us;
	
	// TODO: normalize output... so value between 0 - 1 000 000 ? for pulse duration of PWM ?? not sure

	// TODO: oblicz moc, czyli U / czas
	// P_on = (V_cc * V_cc) / R_heater
	// P_avg = D * P_on 	where D is value between 0-1, that is time when heater is ON during 1 second 
	// (in our example it should be D=0.5)

	return output;
}

void twoPositionalControl(void){
	
}
