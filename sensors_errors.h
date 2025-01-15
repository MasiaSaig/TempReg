#ifndef _SENSORS_ERRORS_H_
#define _SENSORS_ERRORS_H_

#include <LPC17xx.h>

typedef struct{
  uint8_t I2CDisconnected : 1;
  uint8_t heaterDisconnected : 1;
  uint8_t LCDDisconnected : 1;
} sensors_errors_flags;

extern sensors_errors_flags sensors_errors;
  
#endif