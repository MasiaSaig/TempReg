/*
TODO: 
	- implement changing between temperature regulation modes
	- test temperature regulation
	- calculating power consumption by heater	
	- test I2C events on sensor unplugging
(optional)
	- set watchdog
	- setup leds
	- user changing PID amplifications and ?limit?
  - wybierz jedną czcionke w asciiLib.c, asciiLib.h
	- ?? refactor whole code, to use events instead of while(true) loop ?? not really needed
Microprocessor specs:
	CCLK = 100Mhz and PCLK = 25Mhz
*/

#include "UART.h"
#include "I2C_TMP2.h"
#include "PWM.h"
#include "timer.h"
#include "tempRegulation.h"
#include "LCD_screen.h"

#include <LPC17xx.h>
#include <PIN_LPC17xx.h>
#include <Driver_I2C.h>
//#include <Board_LED.h>
#include <Board_Buttons.h>

void handleButtons();

int main(){
	////// INITIALIZATION //////
	//LED_Initialize();
	initUART0();
  
	PWM_Init(1);
	initTimer();
	initLCDScreen();
	UARTprintString("Initializing I2C... \n");
	UARTprintInt(TMP2_Initialize());
	UARTprintString("\nInitialization of I2C completed. ");
	Buttons_Initialize();
  
	/* prepare screen background and unchangable letters */
	// define 'ILI9328' in compiler
	drawConstantDataOnScreen();
  
	////// MAIN LOOP //////
	while(true){
		if(timerStatus.f1ms){
			timerStatus.f1ms = 0; 	// reset flag
		}
		if(timerStatus.f5ms){
			/* handling input buttons */
			timerStatus.f5ms = 0;		// reset flag
		}
		if(timerStatus.f50ms){
			timerStatus.f50ms = 0;	// reset flag
		}
		if(timerStatus.f250ms){
			// handle buttons
			// debouncing handles automatically, since buttons can be pressed only every 50ms
			handleButtons();
			updateDataOnScreen();
			
			timerStatus.f250ms = 0;	// reset flag
		}
		if(timerStatus.f500ms){
			timerStatus.f500ms = 0;	// reset flag
		}
		if(timerStatus.f1s){
			/* main PID functionality, reading and calculating temperature */
			readTemperature();
			UARTprintInt(currentTemperature);
			// calculating temperature regulations and power
			if(PIDControl == 1)
				calculatePID();
			else
				twoPositionalControl();
			
			timerStatus.f1s = 0;		// reset flag
		}
	}
}

void handleButtons(){
	if(Buttons_GetState() & 1) {		// KEY 2
		if(setTemperature <= MAX_TEMPERATURE)
			++setTemperature;
	}else if((Buttons_GetState() & 2)){ 	// KEY 1
		if(setTemperature >= MIN_TEMPERATURE)
			--setTemperature;
	}
}