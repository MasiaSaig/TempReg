#include "tempRegulation.h"
#include "I2C_TMP2.h"
#include "PWM.h"
#include "UART.h"

#define INTEGRAL_LIMIT 10000

// TODO: tune amplifications, calibrate, propert Amplification values
// Amplification values
float Amplification_P = 30.0; //0.1;
float Amplification_I = 0.1; //0.01;
float Amplification_D = 0.1; //0.1;

uint16_t currentTemperature = 0;
int16_t setTemperature = 35;
uint8_t PIDControl = 1;
int16_t temperatureError = 0;
uint16_t heaterPower = 0;

const uint32_t deltaTime_us = 1; // == 1 s
int32_t sumTemperatureError = 0;

void calculatePID(void){
	uint16_t output = 0;
	int16_t temperatureErrorPrev = temperatureError;
	temperatureError = setTemperature - currentTemperature;
	
	// Apply integral only if temperature is lower than 16
	if(temperatureError < 16){
	 	sumTemperatureError += temperatureError*deltaTime_us;
		// boundary
		if(sumTemperatureError >  INTEGRAL_LIMIT) { sumTemperatureError =  INTEGRAL_LIMIT; }
		if(sumTemperatureError < -INTEGRAL_LIMIT) { sumTemperatureError = -INTEGRAL_LIMIT; }
	}else{
		sumTemperatureError = 0;
	}
	
	// P - Proportional
	output += Amplification_P * temperatureError;
	
	// I - Integral
	output += Amplification_I * sumTemperatureError;
	
	// D - Derivative
	output += Amplification_D * (temperatureError - temperatureErrorPrev) / deltaTime_us;
	
	// TODO: oblicz moc, czyli U / czas
	// P_on = (V_cc * V_cc) / R_heater
	// P_avg = D * P_on 	where D is value between 0-1, that is time when heater is ON during 1 second
  heaterPower = output * 0.0439; // output/100 * (6*6/8.2);
  
  UARTprintString(" o");
	UARTprintInt(output);
  
	PWM_SetDutyCycle(output);
}

void twoPositionalControl(void){
	temperatureError = setTemperature - currentTemperature;
	if(currentTemperature > setTemperature){
		heaterPower = 0;
		PWM_SetDutyCycle(0);		// set Pulse duration of PWM to zero, which is turning it off
	}else{
		heaterPower = 50 * 0.0439;
		PWM_SetDutyCycle(100);	// 100% - max time of PWM
	}
}
