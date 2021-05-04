#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_rcc.h"
#include "stm32f7xx_hal_usart.h"
#include "stm32f7xx_hal_uart.h"
#include "stm32f7xx_hal_gpio.h"
#include "string.h"
#include "stdint.h"
#include "stdbool.h"
#include "serial.h"
#include "main.h"

/* USART1 init function */
void USART1_Init(void) {
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  
	if (HAL_UART_Init(&huart1) != HAL_OK) {
    serial_err_handler();
  }
}


int main(void) {
	USART1_Init();
	HAL_UART_MspInit(&huart1);
	
	char data[33];
	uint64_t c = 0;
	while (1) {
		sprintf(data, "0001/9j/4AAQSkZJRgABAQECWAJYAA\n", c++);
		HAL_UART_Transmit(&huart1, data, sizeof(data), 100);
	}
}