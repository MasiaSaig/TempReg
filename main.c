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

#include "UART.h"
#include "I2C_TMP2.h"
#include "PWM.h"
#include "timer.h"
#include "globals.h"

#include <LPC17xx.h>
#include <PIN_LPC17xx.h>
#include <Driver_I2C.h>
#include <Board_LED.h>
#include <Board_Buttons.h>

uint16_t currentTemperature = 0;
uint16_t setTemperature = 25;
uint8_t proportionalControl = 0;

int main(){
	// initialization
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
	
	while(true){
		// buttons handling
		if(Buttons_GetState() & 1) {			// KEY 2
			++setTemperature;
			delay(5);
		}else if((Buttons_GetState() & 2) && (setTemperature > 0)){ 	// KEY 1
			--setTemperature;
			delay(5);
		}
		
		delay(500);
		I2C_Event = 0U;	// clear event
		I2Cdrv->MasterReceive(TMP2_ADDRESS, data, DATA_SIZE, false); 
		while (I2Cdrv->GetStatus().busy);											// Wait until transfer completed
		if (I2Cdrv->GetDataCount () != DATA_SIZE) return -1;		// Check if all data transferred
		convertTemperature();
		UARTsendInt(currentTemperature);
		
		if(proportionalControl){
			// proportional 
		}else{
			// 2-positional control (dwupołożeniowa)
			if(currentTemperature > setTemperature){
				// wyłącz grzejnik - czyli ustaw Pulse Width na zero? albo całkowicie wyłączyć PWM
			}else{
				// jeśli grzejnik wyłączony, włącz go - Pulse Width na 100%, czyli całą długość period
			}
		}
	}
}
