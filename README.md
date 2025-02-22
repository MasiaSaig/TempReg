# Temperature Regulator
It recives current temperature from thermometer and using PWM, regulates duration of time, when the heater is turned ON. 
There are two supported control modes: proportional and two-positional

## Used components
- LPC1768, ARM Cortex M3 with KEY1 and KEY2 buttons 
- dedicated LCD screen with ILI9325 driver
- Pmod TMP2 16-bit, temperature sensor, that uses I2C protocol, together with custom pull-up
- heater, build with resistors, connected to transistor
- UART to output debug logs

This program was written in Keil uVision using C, by Maciej Müller and Piotr Żeberek
