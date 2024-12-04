#include "UART.h"
#include <LPC17xx.h>
#include <PIN_LPC17xx.h>
#include <Driver_I2C.h>

// address is set by selecting jumper blocks on board
// https://digilent.com/reference/pmod/pmodtmp2/reference-manual
#define TMP2_ADDRESS 0x4B
 
// I2C driver instance
extern ARM_DRIVER_I2C            Driver_I2C0;	// 'extern' to be able to access variable, from different file
static ARM_DRIVER_I2C *I2Cdrv = &Driver_I2C0;
 
static volatile uint32_t I2C_Event;
 
// I2C Signal Event function callback
void I2C_SignalEvent (uint32_t event) {
  I2C_Event |= event;		// Save received events
 
  // Optionally, user can define specific actions for an event
  if (event & ARM_I2C_EVENT_TRANSFER_INCOMPLETE) {
    // Less data was transferred than requested
  }
  if (event & ARM_I2C_EVENT_TRANSFER_DONE) {
    // Transfer or receive is finished
  }
  if (event & ARM_I2C_EVENT_ADDRESS_NACK) {
    // Slave address was not acknowledged
  }
  if (event & ARM_I2C_EVENT_ARBITRATION_LOST) {
    // Master lost bus arbitration
  } 
  if (event & ARM_I2C_EVENT_BUS_ERROR) {
    // invalid start/stop position detected
  }
  if (event & ARM_I2C_EVENT_BUS_CLEAR) {
    // Bus clear operation completed
  }
  if (event & ARM_I2C_EVENT_GENERAL_CALL) {
    // Slave was addressed with a general call address
  }
  if (event & ARM_I2C_EVENT_SLAVE_RECEIVE) {
    // Slave addressed as receiver but SlaveReceive operation is not started
  }
  if (event & ARM_I2C_EVENT_SLAVE_TRANSMIT) {
    // Slave addressed as transmitter but SlaveTransmit operation is not started
  }
}
 
// Read I2C connected EEPROM (event driven example)
int32_t EEPROM_Read_Event (uint16_t addr, uint8_t *buf, uint32_t len) {
  uint8_t a[2];
  a[0] = (uint8_t)(addr >> 8);
  a[1] = (uint8_t)(addr & 0xFF);
 
  I2C_Event = 0U;	// Clear event flags before new transfer
 
	// transmit adress of register(a), which is 2 bytes, without sending STOP signal (pending=true)
  I2Cdrv->MasterTransmit(TMP2_ADDRESS, a, 2, true);
 
  while ((I2C_Event & ARM_I2C_EVENT_TRANSFER_DONE) == 0U);							// Wait until transfer completed
  if ((I2C_Event & ARM_I2C_EVENT_TRANSFER_INCOMPLETE) != 0U) return -1;	// Check if all data transferred
 
  I2C_Event = 0U;	// Clear event flags before new transfer
 
  I2Cdrv->MasterReceive(TMP2_ADDRESS, buf, len, false);
 
  while ((I2C_Event & ARM_I2C_EVENT_TRANSFER_DONE) == 0U);							// Wait until transfer completed
  if ((I2C_Event & ARM_I2C_EVENT_TRANSFER_INCOMPLETE) != 0U) return -1;	// Check if all data transferred
 
  return 0;
}
 
// Read I2C connected EEPROM (pooling example)
int32_t EEPROM_Read_Pool (uint16_t addr, uint8_t *buf, int32_t len) {
  uint8_t a[2];
  a[0] = (uint8_t)(addr >> 8);
  a[1] = (uint8_t)(addr & 0xFF);
 
  I2Cdrv->MasterTransmit(TMP2_ADDRESS, a, 2, true);
 
  while (I2Cdrv->GetStatus().busy);							// Wait until transfer completed
  if (I2Cdrv->GetDataCount() != len) return -1;	// Check if all data transferred
 
  I2Cdrv->MasterReceive(TMP2_ADDRESS, buf, len, false);
 
  while (I2Cdrv->GetStatus().busy);								// Wait until transfer completed
  if (I2Cdrv->GetDataCount () != len) return -1;	// Check if all data transferred
 
  return 0;
}
 
// Initialize I2C connected EEPROM
int32_t EEPROM_Initialize (bool pooling) {
  int32_t status;
	uint8_t val;
  //uint8_t val[2];
 
  if (pooling == true) {
    I2Cdrv->Initialize (NULL);
  } else {
    I2Cdrv->Initialize (I2C_SignalEvent);
  }
	
//  I2Cdrv->Initialize(I2C_SignalEvent);
//  I2Cdrv->PowerControl(ARM_POWER_FULL);
//  I2Cdrv->Control(ARM_I2C_OWN_ADDRESS, 0x78);
  I2Cdrv->PowerControl (ARM_POWER_FULL);
  I2Cdrv->Control      (ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_FAST);
  I2Cdrv->Control      (ARM_I2C_BUS_CLEAR, 0);
 
  // Check if EEPROM can be accessed
  if (pooling == true) {
    status = EEPROM_Read_Pool (0x00, &val, 1);
  } else {
    status = EEPROM_Read_Event (0x00, &val, 1);
  }
 
  return (status);
}

int main(){
	initUART0();
 
	sendString("test");
	EEPROM_Initialize(false);
	
  I2C_Event = 0;
	EEPROM_Initialize(true);
	
	const uint32_t dataSize = 2;
	uint8_t data[dataSize];
	while(true){
		// LPC_UART0->THR is a memory address, holding 8bit character, which is going to be send
		// LPC_UART0->THR = 'C';	// sending char
		// sendString("AGH WFIS SW LAB03 TRIAL 1 Lorem ipsum dolor sit amet, Lorem ipsum dolor sit amet\t");	// sending string
		sendString("test");
		EEPROM_Read_Event(0x0, data, dataSize);
		sendInt(data[0]);
		sendInt(data[1]);
	}
}
