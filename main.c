/*
TODO: 
(optional)
  - improve error handling for I2C, LCD display and heater (pin 1.18)
  - check LED displaying power consumption
	- setup watchdog
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

#define CONTROL_SWITCH_THRESHOLD 10

sensors_errors_flags sensors_errors = {0};

typedef struct{
  uint8_t KEY1      : 1;
  uint8_t KEY2      : 1;
  uint8_t BOTHKEYS  : 6;
} pressedButtonsStatus;

pressedButtonsStatus buttonsStatus;

void updateButtonsStatus(void){
  if((Buttons_GetState() & 1) && (Buttons_GetState() & 2)) { // KEY 1 and KEY 2
    ++buttonsStatus.BOTHKEYS;
    buttonsStatus.KEY1 = 0;
    buttonsStatus.KEY2 = 0;
    if(buttonsStatus.BOTHKEYS == CONTROL_SWITCH_THRESHOLD+1)
      buttonsStatus.BOTHKEYS = 0;
  }else if(Buttons_GetState() & 1) {		// KEY 2
		if(setTemperature <= MAX_TEMPERATURE){
			  buttonsStatus.KEY1 = 1;
			  buttonsStatus.KEY2 = 0;
			  buttonsStatus.BOTHKEYS = 0;
		}else{
				buttonsStatus.KEY1 = 0;
			  buttonsStatus.KEY2 = 0;
			  buttonsStatus.BOTHKEYS = 0;
		}
  }else if((Buttons_GetState() & 2)){ 	// KEY 1
		if(setTemperature >= MIN_TEMPERATURE){
			  buttonsStatus.KEY2 = 1;
			  buttonsStatus.KEY1 = 0;
			  buttonsStatus.BOTHKEYS = 0;
    }else{
			buttonsStatus.KEY1 = 0;
			  buttonsStatus.KEY2 = 0;
			  buttonsStatus.BOTHKEYS = 0;
		}
  }else{
	  buttonsStatus.KEY1 = 0;
	  buttonsStatus.KEY2 = 0;
	  buttonsStatus.BOTHKEYS = 0;
  }
}


int main(){
	////// INITIALIZATION //////
	initUART0();
  UARTprintString("UART Initialized.\n\r");
	LED_Initialize();
  UARTprintString("LEDs Initialized.\n\r");
  PWM_Init(1);
  UARTprintString("PWM Initialized.\n\r");
	initTimer();
  UARTprintString("Timer Initialized.\n\r");
	initLCDScreen();    // define 'ILI9328' in compiler
  UARTprintString("LCD Screen Initialized.\n\r");
	UARTprintString("Initializing I2C... ");
	UARTprintInt(TMP2_Initialize());
  UARTprintString(" TMP2 Initialized.\n\r");
	Buttons_Initialize();
  UARTprintString("Buttons Initialized.\n\r");
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
      updateButtonsStatus();
      if(buttonsStatus.KEY1) ++setTemperature;
      if(buttonsStatus.KEY2) --setTemperature;
      if(buttonsStatus.BOTHKEYS == CONTROL_SWITCH_THRESHOLD) PIDControl = !PIDControl;
      updateDataOnScreen();
      
      timerStatus.f50ms = 0;	// reset flag
    }
		if(timerStatus.f250ms){
		  timerStatus.f250ms = 0;	// reset flag
		}
			
		if(timerStatus.f500ms){
			timerStatus.f500ms = 0;	// reset flag
		}
    
		if(timerStatus.f1s){
			/* main PID functionality, reading and calculating temperature */
			readTemperature();
			// calculating temperature regulations and power
			if(PIDControl == 1)
				PWM_SetDutyCycle(calculatePID());
			else
				twoPositionalControl();
			
			// turn ON/OFF LEDs according to current power consumption
			LED_Off();
			uint8_t leds_on = (heaterPower/MAX_HEATER_POWER) * LED_COUNT;
			for(uint8_t i=0; i<leds_on; ++i){
				LED_On(i);
			}
          
        
			timerStatus.f1s = 0;		// reset flag
		}
	}
}
