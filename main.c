#include <stdio.h>
#include "stm32f7xx_hal.h"
#include "cmsis_os.h"
#include "GLCD_Config.h"
#include "Board_GLCD.h"
#include "Board_Touch.h"
#include "screen.h"
#include "serial.h"
#include "tilt.h"
#include "membrane.h"
#include "7seg.h"

#define wait_delay HAL_Delay
extern GLCD_FONT GLCD_Font_6x8;
extern GLCD_FONT GLCD_Font_16x24;


#ifdef __RTX
extern uint32_t os_time;
uint32_t HAL_GetTick(void) {
	return os_time;
}
#endif

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

void wait(uint64_t iters) {
	volatile uint64_t count = 0;
	for (uint64_t i = 0; i < iters; i++) {
		count += i * 2 * i;
	}
}

int main(void) {
	screen_Init();
	
	USART1_Init();
	HAL_UART_MspInit(&huart1);
	tilt_pins_Init();
	initializeMembranePins();
	initSegments();
	enum ALARM_STATE lastState = UNLOCKED;
	displayNum(alarm_state);
	volatile unsigned int trigger_count = 0;
		
	
	unsigned char data[33];
	while (1) {
		
		if (alarm_state == UNLOCKED && clickedOnLockButton()) {
			alarm_state = LOCKED;
			trigger_count = 0;
		} else if (alarm_state == LOCKED && isTiltTriggered()) {
			alarm_state = TRIGGERED;
			trigger_count = 0;
		} else if (alarm_state == TRIGGERED) {
			
			if (trigger_count >= 2000) {
				resetCodeDigits();
				clearCorrectCodeIndicator();
				alarm_state = ALARM;
				trigger_count = 0;
				continue;
			}
			
			int membraneNum = getInput();
			if (membraneNum != -1) {
				while (getInput() != -1) {
					wait(100000);		// wait for user to depress the key
				}
				setCodeDigitNumber(codeCursor++, membraneNum);
				if (codeIsFull()) {
					setStateScreen(alarm_state);
					if (checkCodeCorrect()) {
						drawCorrectCode();
						wait(10000000);
						resetCodeDigits();
						clearCorrectCodeIndicator();
						alarm_state = UNLOCKED;
						resetTilt();
					} else {
						drawIncorrectCode();
						wait(10000000);
						resetCodeDigits();
						clearCorrectCodeIndicator();
						alarm_state = ALARM;
					}
				}
			}
			trigger_count++;
		}
		
		setStateScreen(alarm_state);
		sprintf(data, "%d\n", alarm_state);
		HAL_UART_Transmit(&huart1, data, sizeof(data), 100);
		
		if (lastState != alarm_state) {
			displayNum(alarm_state);
		}
		
		lastState = alarm_state;
	}
	
}
