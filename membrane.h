#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_gpio.h"

#ifndef MEMBRANE_H
#define MEMBRANE_H

void initializeMembranePins (void);
int getInput (void);
int convertPinsToNum(int k, int r);
void turnOn(int pinNo);
void turnOff(int pinNo);
GPIO_PinState readPin(int number);
void setColsIn(void);
void setColsOut(void);
int getInput (void);

#endif