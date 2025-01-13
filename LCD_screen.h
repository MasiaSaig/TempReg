#ifndef _LCD_SCREEN_H_
#define _LCD_SCREEN_H_

#include "LCD_ILI9325.h"
#include <LPC17xx.h>

typedef const unsigned char c_uchar;
typedef unsigned char uchar;

#define DEFAULT_FONT_COLOR LCDWhite
#define DEFAULT_BG_COLOR	 LCDBlack

#define PADDING_TOP  4
#define PADDING_SIDE 8
#define LETTER_WIDTH 8
#define LETTER_HEIGHT 16

/**
 * Initializes LCD screen. 
 *
 * @return void
 */
void initLCDScreen(void);

/**
 * Draws given unsigned char to screen, at given coordinates, using given colors. 
 *
 * Draws given unsigned char to screen using font defined in 'asciiLib.c' written in ASCII 16x8.
 *
 * @param[in] x X coordinate on screen.
 * @param[in] y Y coordinate on screen.
 * @param[in] letter ASCII char to draw on screen.
 * @param[in] fontColor Color of a letter.
 * @param[in] backgroundColor Color of a background, pixels not defined in given letter struct.
 * @return void
 */
void drawLetter(uint16_t x, uint16_t y, c_uchar letter, uint16_t fontColor, uint16_t backgroundColor);
/**
 * Draws given string to screen, at given coordinates, using given colors. 
 *
 * Draws given string of unsigned chars, to screen using font defined in 'asciiLib.c' written in ASCII 16x8.
 * With apropriate colors, given by user.
 *
 * @param[in] x X coordinate on screen.
 * @param[in] y Y coordinate on screen.
 * @param[in] string ASCII chars to draw on screen, must be terminated by '\0' sign.
 * @param[in] fontColor Color of a letter.
 * @param[in] backgroundColor Color of a background, pixels not defined in letter struct.
 * @return void
 */
void drawString(uint16_t x, uint16_t y, c_uchar* string, uint16_t fontColor, uint16_t backgroundColor);
/**
 * Draws given int number to screen, at given coordinates, using given colors. 
 *
 * Draws given integer number, to screen using font defined in 'asciiLib.c' written in ASCII 16x8.
 * With apropriate colors, given by user. It uses drawString() function.
 *
 * @param[in] x X coordinate on screen.
 * @param[in] y Y coordinate on screen.
 * @param[in] number Given number to draw.
 * @param[in] fontColor Color of a letter.
 * @param[in] backgroundColor Color of a background, pixels not defined in letter struct.
 * @return void
 */
void drawIntNumber(uint16_t x, uint16_t y, int16_t number, uint16_t fontColor, uint16_t backgroundColor);
/**
 * Draws given int number to screen, at given coordinates, using given colors. 
 *
 * Draws given float number, with its nth mantysa digits (where n is number provided in precision parameter), to screen 
 * using font, defined in 'asciiLib.c' written in ASCII 16x8. With apropriate colors, given by user. 
 * It uses drawLetter() function.
 *
 * @param[in] x X coordinate on screen.
 * @param[in] y Y coordinate on screen.
 * @param[in] number Given float number to draw.
 * @param[in] precision Number of digits to draw, taken from mantysa.
 * @param[in] fontColor Color of a letter.
 * @param[in] backgroundColor Color of a background, pixels not defined in letter struct.
 * @return void
 */
void drawFloatNumber(uint16_t x, uint16_t y, float number, uint8_t precision, uint16_t fontColor, uint16_t backgroundColor);
/**
 * Sets whole screen to given color. 
 *
 * @param[in] color LCD color to fill screen, with.
 * @return void
 */
void setBackground(uint16_t color);

/**
 *Draws everything, that does not change, with on screen update. 
 *
 * @return void
 */
void drawConstantDataOnScreen(void);
/**
 * Draws things, that update/change while the program is running, mostly temperature numbers, etc. 
 *
 * @return void
 */
void updateDataOnScreen(void);

#endif
