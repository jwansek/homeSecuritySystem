#include <stdio.h>
#include <stdbool.h>
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_gpio.h"
#include "7seg.h"

void displayNum(uint8_t num) {
	// use two functions for this since we don't
	// want to update the previousDisplayed when resetting
	if (resetDisplayFlag) {
		toggleNum(previousDisplayed);
	}
	toggleNum(num);
	previousDisplayed = num;
	resetDisplayFlag = true;
}

void resetDisplay() {
	if (previousDisplayed != NULL) {		
		toggleNum(previousDisplayed);
		resetDisplayFlag = false;
	}
}

Segment getSegment(char pinCode) {
	return segments[pinCode - 97];
}

void segmentToggle(Segment seg) {
	HAL_GPIO_TogglePin(seg.gpio, seg.gpioPin.Pin);
	//seg.gpio->ODR |= seg.gpioPin.Pin;
}

void toggleNum(uint8_t num) {
	for (int i = 0; i <= 7; i++) {
		if (segMappings[num][i] == NULL) {
			break;
		}
		segmentToggle(getSegment(segMappings[num][i]));
	}
}

void initSegments() {
	GPIO_InitTypeDef gpio;
	
	// setup 'a'
	__HAL_RCC_GPIOI_CLK_ENABLE();
	
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_HIGH;
	gpio.Pin = GPIO_PIN_0;
	
	HAL_GPIO_Init(GPIOI, &gpio);
	segments[0].gpioPin = gpio;
	segments[0].gpio = GPIOI;
	segments[0].pinCode = 'a';
	
	// setup 'b'
	__HAL_RCC_GPIOG_CLK_ENABLE();
	
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_HIGH;
	gpio.Pin = GPIO_PIN_6;
	
	HAL_GPIO_Init(GPIOG, &gpio);
	segments[1].gpioPin = gpio;
	segments[1].gpio = GPIOG;
	segments[1].pinCode = 'b';
	
	// setup 'c'
	__HAL_RCC_GPIOH_CLK_ENABLE();
	
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_HIGH;
	gpio.Pin = GPIO_PIN_6;
	
	HAL_GPIO_Init(GPIOH, &gpio);
	segments[2].gpioPin = gpio;
	segments[2].gpio = GPIOH;
	segments[2].pinCode = 'c';
	
	// setup 'd'
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_HIGH;
	gpio.Pin = GPIO_PIN_4;
	
	HAL_GPIO_Init(GPIOB, &gpio);
	segments[3].gpioPin = gpio;
	segments[3].gpio = GPIOB;
	segments[3].pinCode = 'd';
	
	// setup 'e'
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_HIGH;
	gpio.Pin = GPIO_PIN_7;
	
	HAL_GPIO_Init(GPIOG, &gpio);
	segments[4].gpioPin = gpio;
	segments[4].gpio = GPIOG;
	segments[4].pinCode = 'e';
	
	// setup 'f'
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_HIGH;
	gpio.Pin = GPIO_PIN_6;
	
	HAL_GPIO_Init(GPIOC, &gpio);
	segments[5].gpioPin = gpio;
	segments[5].gpio = GPIOC;
	segments[5].pinCode = 'f';
	
	// setup 'g'
	
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_HIGH;
	gpio.Pin = GPIO_PIN_7;
	
	HAL_GPIO_Init(GPIOC, &gpio);
	segments[6].gpioPin = gpio;
	segments[6].gpio = GPIOC;
	segments[6].pinCode = 'g';
}


