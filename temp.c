#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_gpio.h"
#include "math.h"
#include "temp.h"

void temp_pins_Init(void) {
	GPIO_InitTypeDef gpio;

	__HAL_RCC_GPIOA_CLK_ENABLE();

	gpio.Mode = GPIO_MODE_ANALOG;
	gpio.Pull = GPIO_PULLDOWN;
	gpio.Speed = GPIO_SPEED_HIGH;
	gpio.Pin = GPIO_PIN_0;
	
	HAL_GPIO_Init(GPIOA, &gpio);
}

double get_temp(void) {
	double c1 = 0.001129148, c2 = 0.000234125, c3 = 0.0000000876741; //steinhart-hart coeficients for thermistor
	
	int Vo = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
	double resistance = 10000 * (1023.0 / (double)Vo - 1.0);
	return (1.0 / (c1 + c2*log(resistance) + c3*log(resistance)*log(resistance)*log(resistance))) - 273.15;
}

