#ifndef _UART_TEMPERATURE_REGULATOR_H_
#define _UART_TEMPERATURE_REGULATOR_H_

#define WORD_LENGTH_8BIT (0b11)
#define THRE_EMPTY_UART (1<<5)

void initUART0();
void UARTsendString(const char* str);
void UARTsendInt(int x);

#endif