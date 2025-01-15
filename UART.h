#ifndef _UART_TEMPERATURE_REGULATOR_H_
#define _UART_TEMPERATURE_REGULATOR_H_

#define WORD_LENGTH_8BIT (0b11)
#define THRE_EMPTY_UART (1<<5)

/**
 * Initializes UART.
 *
 * Initializes UART on pins P0.2, P0.3 with 8-bit word length and 9600 bauds.
 *
 * @return void
 */
void initUART0(void);

/**
 * Sends given string to connected device (PC), using UART
 *
 * @param[in] str Pointer to char, must be ended with null terminator.
 * @return void
 */
void UARTprintString(const char* str);

/**
 * Sends given integer number to connected device (PC) using UART.
 *
 * @param[in] num Number to send.
 * @return void;
 */
void UARTprintInt(int num);

/**
 * Sends given float number to connected device (PC) using UART.
 *
 * @param[in] num Number to send.
 * @return void;
 */
void UARTprintFloat(float num);

#endif