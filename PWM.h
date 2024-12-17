#ifndef _PWM_HEATER_REGULATOR_H_
#define _PWM_HEATER_REGULATOR_H_

#define PWM_SINGLE_EDGE (0x0)
//#define PWM_TCR_ENABLE ((1<<0) | (1<<3))

#define MAX_PWM_DURATION 1000

void initPWM(void);
void changePulseWidth(unsigned int pulseWidth_us);

#endif
