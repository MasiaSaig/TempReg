#ifndef _LCD_SCREEN_H_
#define _LCD_SCREEN_H_

#include "LCD_ILI9325.h"
#include <LPC17xx.h>

typedef const unsigned char c_uchar;
typedef unsigned char uchar;

#define DEFAULT_FONT_COLOR LCDWhite
#define DEFAULT_BG_COLOR	 LCDBlack

void initLCDScreen(void);

void drawLetter(uint16_t x, uint16_t y, c_uchar letter, 	uint16_t fontColor, uint16_t backgroundColor);
void drawString(uint16_t x, uint16_t y, c_uchar* string, 	uint16_t fontColor, uint16_t backgroundColor);
void drawNumber(uint16_t x, uint16_t y, int16_t number, 	uint16_t fontColor, uint16_t backgroundColor);
void setBackground(uint16_t color);

void drawConstantDataOnScreen(void);
void updateDataOnScreen(void);

#endif
