#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_rcc.h"
#include "stm32f7xx_hal_usart.h"
#include "stm32f7xx_hal_uart.h"
#include "stm32f7xx_hal_gpio.h"
#include "serial.h"
#include "string.h"
#include "math.h"
#include "main.h"

void serial_err_handler() {
	serial_error = true;
}

// must be called this. function declared in stm32f7xx_hal_uart.h
void HAL_UART_MspInit(UART_HandleTypeDef* huart) {

	__HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
	
  GPIO_InitTypeDef GPIO_InitStruct;
  
	if(huart->Instance == USART1) {

    __HAL_RCC_USART1_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
}

void splitIm() {
    uint32_t len = strlen(bliss);
    uint16_t numPackets = (uint16_t)ceil(len / 26) + 1;
    uint16_t packetNo = 0;
    char imPacket[27];

    for (int i = 0; i < len; i += 26) {
        strncpy(imPacket, &bliss[i], 26);
        imPacket[26] = '\0';

        char buff[33];
        sprintf(buff, "%04X%s\n", packetNo++, imPacket);
        HAL_UART_Transmit(&huart1, buff, sizeof(buff), 100);
    }
}
	
