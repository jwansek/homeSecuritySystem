#include "stdbool.h"
#ifndef TILT_H
#define TILT_H

static bool initTiltState;

void tilt_pins_Init(void);
bool isTilted(void);
void resetTilt(void);
bool isTiltTriggered(void);

#endif
