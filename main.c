/*
TODO: 
	- test if i2c works
	- test PWM, ?using led, mabe?
	- implement temperature regulations:	
		- 2-positional
		- proportional
	- display info to screen
	- implement changing between temperature regulation modes
	- test temperature regulation
	- calculating power consumption by heater	
(optional)
	- implement rest of PID functionallity
	- set watchdog

	
Microprocessor specs:
	CCLK = 100Mhz and PCLK = 25Mhz
*/

#include "globals.h"
#include "UART.h"
#include "I2C_TMP2.h"
#include "PWM.h"
#include "timer.h"
#include "PID.h"

#include <LPC17xx.h>
#include <PIN_LPC17xx.h>
#include <Driver_I2C.h>
#include <Board_LED.h>
#include <Board_Buttons.h>


uint16_t currentTemperature = 0;
uint16_t setTemperature = 25;
uint8_t proportionalControl = 0;
uint16_t temperatureError = 0;

void handleButtons();


int main(){
	////// INITIALIZATION //////
	LED_Initialize();
	Buttons_Initialize();
	
	initUART0();
	initPWM();
	
	I2C_Event = 0;
	UARTsendString("Initializing I2C... ");
//	UARTsendInt(TMP2_Initialize(false));
	UARTsendInt(TMP2_Initialize2());
	UARTsendString("Initialization of I2C completed. ");
	convertTemperature();
	UARTsendInt(currentTemperature);
	
	////// MAIN LOOP //////
	uint16_t startTime, stopTime, deltaStartStopTime;
	const uint16_t iterationDuration = 500; // 500ms = 0.5 second
	while(true){
		resetTimer();
		startTime = timestamp();
		
//		handleButtons();
		
		// get temperature data, from sensor
		I2C_Event = 0U;	// clear event
		I2Cdrv->MasterReceive(TMP2_ADDRESS, data, DATA_SIZE, false); 
		while (I2Cdrv->GetStatus().busy);											// Wait until transfer completed
		if (I2Cdrv->GetDataCount () != DATA_SIZE) return -1;	// Check if all data transferred
		convertTemperature();
		UARTsendInt(currentTemperature);
		
		// calculating temperature regulations and power
//		temperatureRegulation();
		
		stopTime = timestamp();
		deltaStartStopTime = stopTime - startTime;
		if(deltaStartStopTime < iterationDuration){
			delay(iterationDuration-deltaStartStopTime);
		}
	}
}

void handleButtons(){
	if(Buttons_GetState() & 1) {		// KEY 2
		++setTemperature;
		delay(5);	// debouncing
	}else if((Buttons_GetState() & 2) && (setTemperature > 0)){ 	// KEY 1
		--setTemperature;
		delay(5);	// debouncing
	}
}