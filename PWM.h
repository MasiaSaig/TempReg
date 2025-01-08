#ifndef _PWM_HEATER_REGULATOR_H_
#define _PWM_HEATER_REGULATOR_H_

#include <LPC17xx.h>

#define SBIT_PC_PWM1 6      // power control bit for PWM1 in PCONP register
#define SBIT_PCLK_PWM1 12   // clock control bit for PWM1 in PCLKSEL0 register

#define SINGLE_EDGE_PWM 0x0 // single edge PWM mode
#define PRESCALER (25-1)    // prescaler value for PWM (1us in default configuration)

//better to get this from the system header file or some structs
#define SCCLK 100000000     // System Core Clock (100 MHz)
#define PCLK (SCCLK/4)      // Peripheral Clock (25 MHz)

/** 
 * Initialization of PWM on pin 1.18
 *
 * Initialization of PWM on pin 1.18 with single-edge mode. 
 * It also turns PWM ON, enables PWM timer and prescaler.
 *
 * @param[in] frequency Frequency of PWM given in Hertz. It determines length of PWM period
 * @return void
 */
void PWM_Init(uint32_t frequency);

/**
 * Set pulse width/duration of PWM
 *
 * @param[n] dutyCycle Takes value from 0 to 100, representing precent of a PWM period, for how long should pulse be active.
 * @return false
 */
void PWM_SetDutyCycle(uint8_t dutyCycle);

#endif
