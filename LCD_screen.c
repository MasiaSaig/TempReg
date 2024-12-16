#include "LCD_screen.h"
#include "Open1768_LCD.h"
#include "LCD_ILI9325.h"
#include "asciiLib.h"

void initLCDScreen(void){
	lcdConfiguration();
	init_ILI9325();
}

void drawLetter(uint16_t x, uint16_t y, unsigned char letter, uint16_t fontColor, uint16_t backgroundColor){
	// ASCII 16x8 - 16 wierszy, 8 kolumn
	// znak/symbol ASCII jest zapisany w tablicy 16 wartosci,
	// gdzie kazda wartosc ma 8 bit�w
	// bit 1 - oznacza pixel znaku (bialy kolor)
	// bit 0 - oznacza pixel tla 	 (czarny kolor)
	unsigned char buffer[16];
	// skopiowanie danych litery wybranej czcionki 1 (mozna wybrac tez 0)
	GetASCIICode(1,buffer, letter);

	for(uint16_t j=0; j<16; ++j){
		lcdSetCursor(x,y+j); 			// resetuje index, so its necessary to also call lcdWriteIndex()
		lcdWriteIndex(DATA_RAM); 
		for(uint16_t i=0; i<8; ++i){
			lcdWriteData((buffer[j]>>(8-i)&0b1) ? fontColor : backgroundColor);
		}
	}
}

void drawString(uint16_t x, uint16_t y, unsigned char* string, uint16_t fontColor, uint16_t backgroundColor){
	for (int i=0; string[i] != '\0'; ++i){
		//if (string[i] == '\0') break;
		drawLetter(x+8*i,y, string[i], fontColor, backgroundColor);
	}
}

void drawNumber(uint16_t x, uint16_t y, int16_t number, uint16_t fontColor, uint16_t backgroundColor){
	int idx = 3;
	unsigned char numberString[5];
	do{
		numberString[idx--] = (char)('0' + number % 10);
		number /= 10;
	}while(number);
	
	for(;idx>=0;--idx) { numberString[idx] = ' '; }
	numberString[4]='\0';	// add string termination
	drawString(x, y, numberString, fontColor, backgroundColor);
}