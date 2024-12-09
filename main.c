/*
TODO: 
	- check if i2c is working
	- configure pwm 
	- to test PWM, configure led
	- improve timer and delay function
	
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


int main(){
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
	UARTsendInt(temperatureValue);
		
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
		I2Cdrv->MasterReceive(TMP2_ADDRESS, data, dataSize, false); 
		while (I2Cdrv->GetStatus().busy);											// Wait until transfer completed
		if (I2Cdrv->GetDataCount () != dataSize) return -1;		// Check if all data transferred
		convertTemperature();
		UARTsendInt(temperatureValue);
	}
}
