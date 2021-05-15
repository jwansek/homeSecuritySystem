#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_rcc.h"
#include "stm32f7xx_hal_usart.h"
#include "stm32f7xx_hal_uart.h"
#include "stm32f7xx_hal_gpio.h"
#include "GLCD_Config.h"
#include "Board_GLCD.h"
#include "Board_Touch.h"
#include "stdio.h"
#include "screen.h"
#include "main.h"

extern GLCD_FONT GLCD_Font_6x8;
extern GLCD_FONT GLCD_Font_16x24;

/**
* System Clock Configuration
*/
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	/* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE();
	/* The voltage scaling allows optimizing the power
	consumption when the device is clocked below the
	maximum system frequency. */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	/* Enable HSE Oscillator and activate PLL
	with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 25;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);
	/* Select PLL as system clock source and configure
	the HCLK, PCLK1 and PCLK2 clocks dividers */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | 
	RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}

void screen_Init(void) {
	HAL_Init();
	SystemClock_Config();
	Touch_Initialize();
	GLCD_Initialize();
	GLCD_ClearScreen();
	GLCD_SetFont(&GLCD_Font_16x24);
	GLCD_SetBackgroundColor(GLCD_COLOR_RED);
	GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
	GLCD_DrawString(0, 0*24, "   Home Security System       ");
	
	GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);
	GLCD_SetForegroundColor(GLCD_COLOR_BLUE);
}

void drawLockButton(uint32_t colour) {
	GLCD_SetForegroundColor(colour);
	if (colour == GLCD_COLOR_RED) {
		GLCD_DrawString(4, 64, "Lock");
	} else {
		GLCD_DrawString(4, 64, "    ");
	}
	GLCD_DrawRectangle(3, 63, 67, 21);
}

bool clickedOnLockButton() {
	Touch_GetState(&tsc_state);
	if (tsc_state.pressed) {
		if (tsc_state.x  >= 3 && tsc_state.x <= 70 && tsc_state.y >= 63 && tsc_state.y <= 63 + 21) {
			return true;
		}
	}
	return false;
}

void drawCodeBoxes(uint32_t colour) {
	GLCD_SetForegroundColor(colour);
	for (uint16_t i = 0; i < 4; i++) {
		if (colour != GLCD_COLOR_NAVY) {
			GLCD_DrawString(3 + (i * 20), 63 + 1, " ");
			GLCD_DrawHLine(3 + (i * 20) + 1, 63 + 23, 14);
		} else {
			if (codeArr[i] != -1 && codeArr[i] != NULL) {
				char buf[8];
				sprintf(buf, "%d", codeArr[i]);
				GLCD_DrawString(3 + (i * 20), 63 + 1, buf);
			} else {
				GLCD_DrawHLine(3 + (i * 20) + 1, 63 + 23, 14);
			}
		}
	}
}

void setCodeDigitNumber(uint16_t cursor, int digit) {
	codeArr[cursor] = digit;
}

void resetCodeDigits() {
	codeCursor = 0;
	for (int i = 0; i < 4; i++) {
		codeArr[i] = -1;
	}
}

bool codeIsFull() {
	bool isFull = true;
	for (int i = 0; i < 4; i++) {
		if (codeArr[i] == -1 || codeArr[i] == NULL) {
			isFull = false;
		}
	}
	return isFull;
}

bool checkCodeCorrect() {
	bool isCorrect = true;
	for (int i = 0; i < 4; i++) {
		if (codeArr[i] != correctCode[i]) {
			isCorrect = false;
		}
	}
	return isCorrect;
}

void drawCorrectCode() {
	GLCD_SetForegroundColor(GLCD_COLOR_GREEN);
	GLCD_DrawString(3, 90, "Unlocking...");
}

void drawIncorrectCode() {
	GLCD_SetForegroundColor(GLCD_COLOR_RED);
	GLCD_DrawString(3, 90, "Incorrect   ");
}

void clearCorrectCodeIndicator() {
	GLCD_DrawString(3, 90, "            ");
}

void setStateScreen(enum ALARM_STATE state) {
	if (state == UNLOCKED) {
		GLCD_SetForegroundColor(GLCD_COLOR_NAVY);
		GLCD_DrawString(0, 30, "Unlocked ");
		drawCodeBoxes(GLCD_COLOR_WHITE);
		drawLockButton(GLCD_COLOR_RED);
	} else if (state == LOCKED) {
		GLCD_SetForegroundColor(GLCD_COLOR_GREEN);
		GLCD_DrawString(0, 30, "Locked   ");
		drawLockButton(GLCD_COLOR_WHITE);
		drawCodeBoxes(GLCD_COLOR_WHITE);
	} else if (state == TRIGGERED) {
		GLCD_SetForegroundColor(GLCD_COLOR_MAGENTA);
		GLCD_DrawString(0, 30, "Triggered");
		drawLockButton(GLCD_COLOR_WHITE);
		drawCodeBoxes(GLCD_COLOR_NAVY);
	} else if (state == ALARM) {
		GLCD_SetForegroundColor(GLCD_COLOR_RED);
		GLCD_DrawString(0, 30, "Alarm    ");
		drawLockButton(GLCD_COLOR_WHITE);
		drawCodeBoxes(GLCD_COLOR_WHITE);
	}
}
		
	