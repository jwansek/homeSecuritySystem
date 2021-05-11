#include "GLCD_Config.h"
#include "Board_GLCD.h"
#include "Board_Touch.h"
#include "main.h"

#ifndef SCREEN_H
#define SCREEN_H

static TOUCH_STATE tsc_state;
static int codeArr[4] = {-1, -1, -1, -1};
static int codeCursor = 0;

void SystemClock_Config(void);
void screen_Init(void);
void setStateScreen(enum ALARM_STATE state);
void drawLockButton(uint32_t colour);
void drawCodeBoxes(uint32_t colour);
void drawCodeBoxNumber(uint16_t cursor, int digit);
	

#endif

