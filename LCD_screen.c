#include "LCD_screen.h"
#include "Open1768_LCD.h"
#include "asciiLib.h"
#include "tempRegulation.h"
#include "I2C_TMP2.h"

void initLCDScreen(void){
	lcdConfiguration();
	init_ILI9325();
}


void drawLetter(uint16_t x, uint16_t y, c_uchar letter, uint16_t fontColor, uint16_t backgroundColor){
	// ASCII 16x8 - 16 rows, 8 columns
	// ASCII sign written in table of 16 values, where every value consists of 8 bits.
	// bit 1 == letter pixel (white) 
	// bit 0 == background pixel (black)
	/*
			11111111
			10000001
			10000001
			...					==	0
			10000001
			11111111
	*/
	unsigned char buffer[16];
  // copying letter data of chosen font 1 (you can change font to 0)
	GetASCIICode(1,buffer, letter);

	for(uint16_t j=0; j<16; ++j){
		lcdSetCursor(x,y+j); 			// resets index, so its necessary to also call lcdWriteIndex()
		lcdWriteIndex(DATA_RAM); 
		for(uint16_t i=0; i<8; ++i){
			lcdWriteData((buffer[j]>>(8-i)&0b1) ? fontColor : backgroundColor);
		}
	}
}

void drawString(uint16_t x, uint16_t y, c_uchar* string, uint16_t fontColor, uint16_t backgroundColor){
	for (int i=0; string[i] != '\0'; ++i){
		drawLetter(x+8*i,y, string[i], fontColor, backgroundColor);
	}
}

void drawIntNumber(uint16_t x, uint16_t y, int16_t number, uint16_t fontColor, uint16_t backgroundColor){
  // draw minus ('-') if number if lower than zero
  if(number < 0){
    drawLetter(x,y,'-',fontColor,backgroundColor);
    number *= -1;
    x += LETTER_WIDTH;
  }
  
  int8_t digits = 0;
  int16_t number_cpy = number;
  // count digits
  do{
    number_cpy = number_cpy / 10;
    ++digits;
  }while(number_cpy);
  
  // draw digits, starting from most-significant one
  int modulator = 1;
  for(uint8_t i=1;i<digits; ++i) { modulator *= 10; }
  do{
    drawLetter(x,y,'0'+number/modulator,fontColor,backgroundColor);
    x += LETTER_WIDTH;
    --digits;
    modulator /= 10;
  }while(number);
}

void drawFloatNumber(uint16_t x, uint16_t y, float number, uint8_t precision, uint16_t fontColor, uint16_t backgroundColor){
  // draw minus ('-') if number if lower than zero
  if(number < 0){
    drawLetter(x,y,'-',fontColor,backgroundColor);
    number *= -1;
    x += LETTER_WIDTH;
  }
  
  int8_t digits = 0;
  uint16_t number_cpy = number;
  // count digits
  do{
    number_cpy = number_cpy / 10;
    ++digits;
  }while(number_cpy);
  
  for(uint8_t i=0; i<precision; ++i) { number *= 10; }
  digits += precision;
  // draw digits, starting from most-significant one
  int modulator = 1;
  for(uint8_t i=1;i<digits; ++i) { modulator *= 10; }
  do{
    drawLetter(x,y,'0'+(int)(number/modulator),fontColor,backgroundColor);
    x += LETTER_WIDTH;
    --digits;
    modulator /= 10;
  }while(digits > precision);
  
  // draw dot
  drawLetter(x, y, '.', fontColor, backgroundColor);
  x += LETTER_WIDTH;
  
  for(uint8_t i=0; i<precision; ++i){
    drawLetter(x,y,'0'+(int)(number/modulator),fontColor,backgroundColor);
    x += LETTER_WIDTH;
    --digits;
    modulator /= 10;
  }
}

void setBackground(uint16_t backgroundColor){
	lcdSetCursor(0,0);
	lcdWriteIndex(DATA_RAM);
	uint32_t pixelsNum = LCD_MAX_X*LCD_MAX_Y;
	for(uint32_t i=0; i<pixelsNum; ++i){
		lcdWriteData(backgroundColor);
	}
}


void updateDataOnScreen(void){
	drawIntNumber(PADDING_SIDE+13*LETTER_WIDTH,PADDING_TOP,                                 currentTemperature, DEFAULT_FONT_COLOR, DEFAULT_BG_COLOR);
	drawIntNumber(PADDING_SIDE+17*LETTER_WIDTH,PADDING_TOP+LETTER_HEIGHT+PADDING_TOP,       setTemperature,			DEFAULT_FONT_COLOR, DEFAULT_BG_COLOR);
	drawIntNumber(PADDING_SIDE+19*LETTER_WIDTH,PADDING_TOP+(LETTER_HEIGHT+PADDING_TOP)*2,   temperatureError,		DEFAULT_FONT_COLOR, DEFAULT_BG_COLOR);
	drawFloatNumber(PADDING_SIDE+07*LETTER_WIDTH,PADDING_TOP+(LETTER_HEIGHT+PADDING_TOP)*3, heaterPower, 2,		  DEFAULT_FONT_COLOR, DEFAULT_BG_COLOR);
  if(PIDControl)
    drawString(PADDING_SIDE+14*LETTER_WIDTH,PADDING_TOP+(LETTER_HEIGHT+PADDING_TOP)*4, (c_uchar*)"PID           ", DEFAULT_FONT_COLOR, DEFAULT_BG_COLOR);
  else
    drawString(PADDING_SIDE+14*LETTER_WIDTH,PADDING_TOP+(LETTER_HEIGHT+PADDING_TOP)*4, (c_uchar*)"two-positional", DEFAULT_FONT_COLOR, DEFAULT_BG_COLOR);
}

void drawConstantDataOnScreen(void){
  setBackground(DEFAULT_BG_COLOR);
	drawString(PADDING_SIDE,PADDING_TOP,                               (c_uchar*)"Temperature: ",       DEFAULT_FONT_COLOR, DEFAULT_BG_COLOR);
	drawString(PADDING_SIDE,PADDING_TOP+LETTER_HEIGHT+PADDING_TOP,     (c_uchar*)"Set Temperature: ",   DEFAULT_FONT_COLOR, DEFAULT_BG_COLOR);
	drawString(PADDING_SIDE,PADDING_TOP+(LETTER_HEIGHT+PADDING_TOP)*2, (c_uchar*)"Temperature Error: ", DEFAULT_FONT_COLOR, DEFAULT_BG_COLOR);
	drawString(PADDING_SIDE,PADDING_TOP+(LETTER_HEIGHT+PADDING_TOP)*3, (c_uchar*)"Power: ",             DEFAULT_FONT_COLOR, DEFAULT_BG_COLOR);
  drawString(PADDING_SIDE,PADDING_TOP+(LETTER_HEIGHT+PADDING_TOP)*4, (c_uchar*)"Control Mode: ",      DEFAULT_FONT_COLOR, DEFAULT_BG_COLOR);
}
	