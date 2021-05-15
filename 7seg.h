#ifndef SEVEN_SEG_H
#define SEVEN_SEG_H
#include <stdbool.h>
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_gpio.h"

static char segMappings[16][8] = {
	{'a', 'b', 'c', 'd', 'e', 'f', NULL},			// 0x0
	{'b', 'c', NULL},													// 0x1
	{'a', 'b', 'g', 'e', 'd', NULL},					// 0x2
	{'a', 'b', 'g', 'c', 'd', NULL},					// 0x3
	{'f', 'g', 'b', 'c', NULL},								// 0x4
	{'a', 'f', 'g', 'c', 'd', NULL},					// 0x5
	{'a', 'f', 'g', 'c', 'd', 'e', NULL},			// 0x6	
	{'f', 'a', 'b', 'c', NULL},								// 0x7
	{'a', 'b', 'c', 'd', 'e', 'f', 'g', NULL},// 0x8
	{'a', 'b', 'c', 'd', 'f', 'g', NULL},			// 0x9
	{'a', 'b', 'c', 'e', 'f', 'g', NULL},			// 0xA
	{'c', 'd', 'e', 'f', 'g', NULL},					// 0xB
	{'a', 'f', 'e', 'd', NULL},								// 0xC
	{'b', 'c', 'd', 'e', 'g', NULL},					// 0xD
	{'a', 'd', 'e', 'f', 'g', NULL},					// 0xE
	{'a', 'f', 'g', 'e', NULL}								// 0xF
};

static uint8_t previousDisplayed;
static bool resetDisplayFlag = false;

typedef struct {
	GPIO_InitTypeDef gpioPin;
	GPIO_TypeDef* gpio;
	char pinCode;
} Segment;

static Segment segments[7];

void displayNum(uint8_t num);
void resetDisplay(void);
void segmentToggle(Segment seg);
Segment getSegment(char pinCode);
void initSegments(void);
void toggleNum(uint8_t num);

#endif