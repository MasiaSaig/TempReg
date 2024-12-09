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

#include <LPC17xx.h>
#include <PIN_LPC17xx.h>
#include <Driver_I2C.h>
#include <Board_LED.h>


int main(){
	LED_Initialize();
	
	initUART0();
	initPWM();
 
	I2C_Event = 0;
	UARTsendString("Initializing I2C... ");
//	UARTsendInt(TMP2_Initialize(false));
	UARTsendInt(TMP2_Initialize2());
	UARTsendString("Initialization of I2C completed. ");
	convertTemperature();
	UARTsendInt(temperatureValue);
	
	// TODO: temporary delay (delete later)
	uint32_t returnCode = SysTick_Config(SystemCoreClock / 10);
	if(returnCode != 0) { /* handle error */ }
		
	while(true){
		// LPC_UART0->THR is a memory address, holding 8bit character, which is going to be send
		// LPC_UART0->THR = 'C';										// sending char
		// UARTsendString("AGH WFIS SW.\t");				// sending string
		delay(500);
		I2C_Event = 0U;	// clear event
		I2Cdrv->MasterReceive(TMP2_ADDRESS, data, dataSize, false); 
		while (I2Cdrv->GetStatus().busy);											// Wait until transfer completed
		if (I2Cdrv->GetDataCount () != dataSize) return -1;		// Check if all data transferred
		convertTemperature();
		UARTsendInt(temperatureValue);
	}
}
