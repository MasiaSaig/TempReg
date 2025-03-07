#include "I2C_TMP2.h"
#include "UART.h"
#include "sensors_errors.h"

ARM_DRIVER_I2C *I2Cdrv = &Driver_I2C0;
volatile uint32_t I2C_Event = 0U;

uint8_t tempData[TEMP_DATA_SIZE] = {0};

// I2C Signal Event function callback
void I2C_SignalEvent(uint32_t event) {
  I2C_Event |= event;			// Save received events
  // Optionally, user can define specific actions for an event
  if (event & ARM_I2C_EVENT_TRANSFER_INCOMPLETE) { /* Less data was transferred than requested */ }
  if (event & ARM_I2C_EVENT_TRANSFER_DONE) { /* Transfer or receive is finished */ }
  if (event & ARM_I2C_EVENT_ADDRESS_NACK) { 
    UARTprintString("I2C Slave address was not acknowledged\t"); 
    sensors_errors.I2CDisconnected = 1;
  }else { sensors_errors.I2CDisconnected = 0; }
  if (event & ARM_I2C_EVENT_ARBITRATION_LOST) { /* Master lost bus arbitration */ } 
  if (event & ARM_I2C_EVENT_BUS_ERROR) { 
    UARTprintString("I2C invalid start/stop position detected\t"); 
  }
  if (event & ARM_I2C_EVENT_BUS_CLEAR) { /* Bus clear operation completed */ }
  if (event & ARM_I2C_EVENT_GENERAL_CALL) { /* Slave was addressed with a general call address */ }
  if (event & ARM_I2C_EVENT_SLAVE_RECEIVE) { /* Slave addressed as receiver but SlaveReceive operation is not started */ }
  if (event & ARM_I2C_EVENT_SLAVE_TRANSMIT) { /* Slave addressed as transmitter but SlaveTransmit operation is not started */ }
}

int32_t TMP2_Read_Event(uint16_t addr, uint8_t *buf, uint32_t len) {
  uint8_t a[2];
  a[0] = (uint8_t)(addr >> 8);
  a[1] = (uint8_t)(addr & 0xFF);
 
  I2C_Event = 0U;			// Clear event flags before new transfer
	// transmit adress of register(a), which is 2 bytes, without sending STOP signal (pending=true)
  I2Cdrv->MasterTransmit(TMP2_ADDRESS, a, 2, true);
  while ((I2C_Event & ARM_I2C_EVENT_TRANSFER_DONE) == 0U);								// Wait until transfer completed
  if ((I2C_Event & ARM_I2C_EVENT_TRANSFER_INCOMPLETE) != 0U) return -1;		// Check if all data transferred
 
  I2C_Event = 0U;	// Clear event flags before new transfer
  I2Cdrv->MasterReceive(TMP2_ADDRESS, buf, len, false);
 
  while ((I2C_Event & ARM_I2C_EVENT_TRANSFER_DONE) == 0U);								// Wait until transfer completed
  if ((I2C_Event & ARM_I2C_EVENT_TRANSFER_INCOMPLETE) != 0U) return -1;		// Check if all data transferred
 
  return 0;
}
 
// Initialize I2C connected EEPROM
int32_t TMP2_Initialize(void) {
	uint8_t val[2];
 
  I2Cdrv->Initialize (I2C_SignalEvent);
	
  I2Cdrv->PowerControl (ARM_POWER_FULL);
  I2Cdrv->Control      (ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_STANDARD);		// ARM_I2C_BUS_SPEED_FAST
  I2Cdrv->Control      (ARM_I2C_BUS_CLEAR, 0);
 
  //status = TMP2_Read_Event(0x0, &val, 1);
	
	uint8_t a[2];
  a[0] = (uint8_t)(0x0 >> 8);
  a[1] = (uint8_t)(0x0 & 0xFF);
 
  I2C_Event = 0U;			// Clear event flags before new transfer
	// transmit adress of register(a), which is 2 bytes, without sending STOP signal (pending=true)
  I2Cdrv->MasterTransmit(TMP2_ADDRESS, a, 2, true);
  while ((I2C_Event & ARM_I2C_EVENT_TRANSFER_DONE) == 0U);								// Wait until transfer completed
  if ((I2C_Event & ARM_I2C_EVENT_TRANSFER_INCOMPLETE) != 0U) return -1;		// Check if all data transferred
 
  I2C_Event = 0U;	// Clear event flags before new transfer
  I2Cdrv->MasterReceive(TMP2_ADDRESS, val, 2, false);
 
  while ((I2C_Event & ARM_I2C_EVENT_TRANSFER_DONE) == 0U);								// Wait until transfer completed
  if ((I2C_Event & ARM_I2C_EVENT_TRANSFER_INCOMPLETE) != 0U) return -1;		// Check if all data transferred

  return 0;
}

/* https://digilent.com/reference/_media/reference/pmod/pmodtmp2/pmodtmp2_rm.pdf section: Quick Start Operation */
// Converts MSB and LSB into temperature value, represented in Celsius degrees
void convertTemperature(void){
	currentTemperature = (((tempData[0]<<8) | tempData[1])>>3) * 0.0625;
}

bool readTemperature(void){
	I2C_Event = 0U;	// clear event
	I2Cdrv->MasterReceive(TMP2_ADDRESS, tempData, TEMP_DATA_SIZE, false); 
	while (I2Cdrv->GetStatus().busy);											// Wait until transfer completed
	if (I2Cdrv->GetDataCount () != TEMP_DATA_SIZE) false;	// Check if all data transferred
	convertTemperature();
	return true;
}
