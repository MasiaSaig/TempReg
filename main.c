#include "UART.h"
#include <LPC17xx.h>
#include <PIN_LPC17xx.h>

int main(){
	initUART0();
	
	while(true){
		// LPC_UART0->THR is a memory address, holding 8bit character, which is going to be send
		// LPC_UART0->THR = 'C';	// sending char
		// sendString("AGH WFIS SW LAB03 TRIAL 1 Lorem ipsum dolor sit amet, Lorem ipsum dolor sit amet\t");	// sending string
		sendInt(-456789);
	}
}
