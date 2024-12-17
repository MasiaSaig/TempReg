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
	- wybierz jedną czcionke w asciiLib.c, asciiLib.h
(optional)
	- implement rest of PID functionallity
	- set watchdog
	- refactor whole code, to use events instead of while(true) loop
	- user changing PID amplifications and ?limit?

	
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
#include <Board_LED.h>
#include <Board_Buttons.h>

typedef unsigned char* uchar_p;

void handleButtons();

int main(){
	////// INITIALIZATION //////
	LED_Initialize();
	Buttons_Initialize();
	
	initUART0();
	initPWM();
	initTimer();
	initLCDScreen();
	
	I2C_Event = 0;
	UARTprintString("Initializing I2C... \n");
//	UARTsendInt(TMP2_Initialize(false));
	UARTprintInt(TMP2_Initialize2());
	UARTprintString("\nInitialization of I2C completed. ");
	
	/* prepare screen background and unchangable letters */
	/*
	setBackground(DEFAULT_BG_COLOR);
	drawString(8,8,  (uchar_p)"Temperature: " 			, DEFAULT_FONT_COLOR, DEFAULT_BG_COLOR);
	drawString(8,32, (uchar_p)"Set Temperature: "		, DEFAULT_FONT_COLOR, DEFAULT_BG_COLOR);
	drawString(8,56, (uchar_p)"Temperature Error: " , DEFAULT_FONT_COLOR, DEFAULT_BG_COLOR);
	drawString(8,80, (uchar_p)"Power: "			, DEFAULT_FONT_COLOR, DEFAULT_BG_COLOR);
	*/
	
	////// MAIN LOOP //////
	while(true){
		if(timerStatus.f1ms){
			timerStatus.f1ms = 0;		// !MUST reset 1ms flag!
		}
		if(timerStatus.f5ms){
			/* handling input buttons */
			// debouncing handles automatically, since buttons can be pressed only every 5ms
			/*
			handleButtons();
			*/
			timerStatus.f5ms = 0;
		}
		if(timerStatus.f50ms){
			timerStatus.f50ms = 0;
		}
		if(timerStatus.f500ms){
			/* update screen */
			/*
			drawNumber(112,8,  currentTemperature,	DEFAULT_FONT_COLOR, DEFAULT_BG_COLOR);
			drawNumber(144,32, setTemperature,			DEFAULT_FONT_COLOR, DEFAULT_BG_COLOR);
			drawNumber(160,56, temperatureError,		DEFAULT_FONT_COLOR, DEFAULT_BG_COLOR);
			drawNumber(64,80,  heaterPower,					DEFAULT_FONT_COLOR, DEFAULT_BG_COLOR);
			*/
			timerStatus.f500ms = 0;
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
			
			
			timerStatus.f1s = 0;
		}
		
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
		if(setTemperature <= MAX_TEMPERATURE)
			++setTemperature;
	}else if((Buttons_GetState() & 2) && (setTemperature > 0)){ 	// KEY 1
		if(setTemperature >= MIN_TEMPERATURE)
			--setTemperature;
	}
}