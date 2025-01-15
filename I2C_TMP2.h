#ifndef _TMP2_I2C_TEMPERATURE_REGULATOR_H_
#define _TMP2_I2C_TEMPERATURE_REGULATOR_H_

#include <Driver_I2C.h>

// 7-bit address is set by selecting jumper blocks on board
// https://digilent.com/reference/pmod/pmodtmp2/reference-manual
#define TMP2_ADDRESS 0x4B				//!< Address(7-bit) of a slave TMP2, with both jump pins(JP1/2) open.
#define TMP2_TEMP_ADDRESS 0x00	//!< Address of data(MSb) inside TMP2.
 
#define TEMP_DATA_SIZE 2
extern uint8_t tempData[TEMP_DATA_SIZE];	//!< Temperature data, holding bits(MSb, LSb) taken from TMP2.
 
// I2C driver instance
extern ARM_DRIVER_I2C Driver_I2C0;
extern ARM_DRIVER_I2C *I2Cdrv;
extern volatile uint32_t I2C_Event;		//!< Event from I2C protocol, which is handled in I2C_SignalEvent() function/

extern float currentTemperature;		//!< Current temperature, read by sensor.

/**
 * Initialize TMP2 to use I2C protocot. 
 *
 * @return Initialization status, 0 means initialization was succesful, otherwise, there was some error.
 */
int32_t TMP2_Initialize(void);

/*! Function used in initialization, which sends address of slave and reads data(temperature).  */
//int32_t TMP2_Read_Event(uint16_t addr, uint8_t *buf, uint32_t len);
/**
 * Read events from TMP2 and handles them. 
 *
 * @return void
 */
void I2C_SignalEvent(uint32_t event);

/**
 * Read temperature data, from TMP2 and convert it to Celsius degrees.
 *
 * @return False if not all data were transfered.
 */
bool readTemperature(void);
/**
 * Convert temperature data from TMP2 into Celsius degrees. 
 *
 * @return void
 */
inline void convertTemperature(void);

#endif