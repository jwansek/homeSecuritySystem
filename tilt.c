#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_gpio.h"
#include "stdbool.h"
#include "tilt.h"

void tilt_pins_Init(void) {
	GPIO_InitTypeDef gpio;

	__HAL_RCC_GPIOI_CLK_ENABLE();

	gpio.Mode = GPIO_MODE_INPUT;
	gpio.Pull = GPIO_PULLDOWN;
	gpio.Speed = GPIO_SPEED_HIGH;
	gpio.Pin = GPIO_PIN_3;
	
	HAL_GPIO_Init(GPIOI, &gpio);
}

bool isTilted(void) {
	return HAL_GPIO_ReadPin(GPIOI, GPIO_PIN_3);
}
