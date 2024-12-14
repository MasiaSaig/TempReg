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
	- test I2C events on sensor unplugging
(optional)
	- implement rest of PID functionallity
	- set watchdog
	- refactor whole code, to use events instead of while(true) loop
	- user changing PID amplifications and ?limit?

	
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
uint16_t setTemperature = 35;
uint8_t proportionalControl = 0;
uint16_t temperatureError = 0;

void handleButtons();

int main(){
	////// INITIALIZATION //////
	LED_Initialize();
	Buttons_Initialize();
	
	initUART0();
	initPWM();
	initTimer();
	
	I2C_Event = 0;
	UARTprintString("Initializing I2C... \n");
//	UARTsendInt(TMP2_Initialize(false));
	UARTprintInt(TMP2_Initialize2());
	UARTprintString("\nInitialization of I2C completed. ");
	
	////// MAIN LOOP //////
	uint32_t startTime, stopTime, deltaStartStopTime;
	const uint32_t iterationDuration = 500; // 500ms = 0.5 second
	while(true){
		handleButtons();
		
		readTemperature();
		UARTprintInt(currentTemperature);
		
		// calculating temperature regulations and power
//		temperatureRegulation();
		
		
		// TODO: test timer
		/*resetTimer();
		startTime = timestamp();
		delay(iterationDuration);
		stopTime = timestamp();
		deltaStartStopTime = stopTime - startTime;
		if(deltaStartStopTime < iterationDuration){
			UARTprintInt(iterationDuration-deltaStartStopTime);
			delay(iterationDuration-deltaStartStopTime);
		}*/

		// TODO: test PWM and PID
		/* changePulseWidth(5000);
		 changePulseWidth(15000);*/
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