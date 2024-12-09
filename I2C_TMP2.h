#ifndef _TMP2_I2C_TEMPERATURE_REGULATOR_H_
#define _TMP2_I2C_TEMPERATURE_REGULATOR_H_

#include <Driver_I2C.h>

// address is set by selecting jumper blocks on board
// https://digilent.com/reference/pmod/pmodtmp2/reference-manual
#define TMP2_ADDRESS 0x4B
#define TMP2_TEMP_ADDRESS 0x0
 
#define DATA_SIZE 2
extern uint8_t data[DATA_SIZE];
 
// I2C driver instance
extern ARM_DRIVER_I2C Driver_I2C0;
extern ARM_DRIVER_I2C *I2Cdrv;
extern volatile uint32_t I2C_Event;

int32_t TMP2_Initialize(bool pooling);
int32_t TMP2_Read_Pool(uint16_t addr, uint8_t *buf, int32_t len);
int32_t TMP2_Read_Event(uint16_t addr, uint8_t *buf, uint32_t len);

void I2C_SignalEvent (uint32_t event);
void convertTemperature();



// TODO: DO SPRAWDZENIA
int32_t TMP2_Initialize2();
int32_t TMP2_Read_Event_NoAdr(uint8_t *buf, uint32_t len);

#endif