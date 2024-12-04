#include "UART.h"
#include <LPC17xx.h>
#include <PIN_LPC17xx.h>

void sendString(const char* str){
	for (int i=0; ; ++i){
		if (str[i] == '\0') break;
		LPC_UART0->THR = str[i];
		// bit 5:		THRE is set immediately upon detection of an empty UARTn THR and is cleared on a UnTHR write. 
		//					0 - UnTHRE contains valid data
		//					1 - UnTHR is empty
		while(!(LPC_UART0->LSR & THRE_EMPTY_UART));
	}
}

void sendInt(int x){
	if(x<0){						// send minus, if number is lower than 0
		sendString("-");
		x*=-1;
	}	

	int tempX=x, divider=1;				// calculating divider
	while(tempX/=10) { divider *= 10; }	
	while(divider){								// using divider to show digits starting from left one
		LPC_UART0->THR = (char)('0' + x / divider);	
		while(!(LPC_UART0->LSR & THRE_EMPTY_UART));
		x %= divider;
		divider /= 10;
	}
}

int main(){
	/* (Chapter 8 - Table 79) */
	// pin  		= P0.3 (RxD Recive Data)
	PIN_Configure(0, 3, 1, 0b00, 0);	// RxD
	// port			= P0.2 (TxD Transmit Data)
	PIN_Configure(0, 2, 1, 0b00, 0);	// TxD
	
	/* (Chapter 14 - Table 279) */
	LPC_UART0->LCR = (1<<7) | WORD_LENGTH_8BIT;	// enable DLAB and set word length to 8-bit
	LPC_UART0->DLL = 163;										// [DL=25000Hz / (16*B)], for bauds B=9600 (error=0,14%)
	LPC_UART0->DLM = 0;											// little error, so we can set it to 0 (by default)
	LPC_UART0->LCR = WORD_LENGTH_8BIT;			// disable DLAB (keeping word length to 8-bit)
	
	while(true){
		// LPC_UART0->THR is a memory address, holding 8bit character, which is going to be send
		// LPC_UART0->THR = 'C';	// sending char
		// sendString("AGH WFIS SW LAB03 TRIAL 1 Lorem ipsum dolor sit amet, Lorem ipsum dolor sit amet\t");	// sending string
		sendInt(-456789);
	}
}
