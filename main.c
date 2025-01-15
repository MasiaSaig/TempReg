/*
TODO: 
	- tune PID amplifications
	- test I2C events on sensor unplugging
  - gather data to plot them for documentation
  - check calculated power consumption
  - check LED displaying power consumption
(optional)
	- setup watchdog
	- setup leds, to show power consumption
	- user changing PID amplifications and ?limit?
  - wybierz jedną czcionke w asciiLib.c, asciiLib.h
  
Microprocessor specs:
	CCLK = 100Mhz and PCLK = 25Mhz
*/

#include "UART.h"
#include "I2C_TMP2.h"
#include "PWM.h"
#include "timer.h"
#include "tempRegulation.h"
#include "LCD_screen.h"
#include "sensors_errors.h"

#include <LPC17xx.h>
#include <PIN_LPC17xx.h>
#include <Driver_I2C.h>
#include <Board_LED.h>
#include <Board_Buttons.h>

sensors_errors_flags sensors_errors = {0};

typedef struct{
  uint8_t KEY1      : 1;
  uint8_t KEY2      : 1;
  uint8_t BOTHKEYS  : 6;
} pressedButtonsStatus;
pressedButtonsStatus buttonsStatus;

void updateButtonsStatus();

int main(){
	////// INITIALIZATION //////
	LED_Initialize();
	initUART0();
	PWM_Init(1);
	initTimer();
	initLCDScreen();    // define 'ILI9328' in compiler
	UARTprintString("Initializing I2C... \n");
	UARTprintInt(TMP2_Initialize());
	UARTprintString("\nInitialization of I2C completed. ");
	Buttons_Initialize();
  buttonsStatus.KEY1 = 0;
  buttonsStatus.KEY2 = 0;
  buttonsStatus.BOTHKEYS = 0;
  
	// prepare screen background and unchangable letters
	drawConstantDataOnScreen();
  
  uint8_t LED_COUNT = LED_GetCount();
  
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
      // handle buttons
      if(buttonsStatus.KEY1) ++setTemperature;
      if(buttonsStatus.KEY2) --setTemperature;
      updateButtonsStatus();  // <---------------------|  xD
      updateDataOnScreen();                      //    |
      timerStatus.f50ms = 0;	// reset flag      //    |
    }                                            //    |
		if(timerStatus.f250ms){                      //    |
      // if user holds both buttons for atleast 40*50ms == 2s, then switch temperature regulation mode
      if(buttonsStatus.BOTHKEYS >= 40) PIDControl = !PIDControl;
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
      // turn ON/OFF LEDs according to current power consumption
      for(float i=0; i<LED_COUNT; ++i){
        if((heaterPower/MAX_HEATER_POWER) * (i+1) < heaterPower)
          LED_On(i);
        else
          LED_Off(i);
      }
			
			timerStatus.f1s = 0;		// reset flag
		}
	}
}

void updateButtonsStatus(){
  if((Buttons_GetState() & 1) && (Buttons_GetState() & 2)) {
    ++buttonsStatus.BOTHKEYS;
    buttonsStatus.KEY1 = 0;
    buttonsStatus.KEY2 = 0;
    if(buttonsStatus.BOTHKEYS > 40) buttonsStatus.BOTHKEYS = 0;
  }else if(Buttons_GetState() & 1) {		// KEY 2
		if(setTemperature <= MAX_TEMPERATURE){
			  buttonsStatus.KEY1 = 1;
			  buttonsStatus.KEY2 = 0;
			  buttonsStatus.BOTHKEYS = 0;
		  }
  }else if((Buttons_GetState() & 2)){ 	// KEY 1
		if(setTemperature >= MIN_TEMPERATURE){
			  buttonsStatus.KEY2 = 1;
			  buttonsStatus.KEY1 = 0;
			  buttonsStatus.BOTHKEYS = 0;
		  }
  }else{
	  buttonsStatus.KEY1 = 0;
	  buttonsStatus.KEY2 = 0;
	  buttonsStatus.BOTHKEYS = 0;
  }
}
