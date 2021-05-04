#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_rcc.h"
#include "stm32f7xx_hal_usart.h"
#include "stm32f7xx_hal_uart.h"
#include "stm32f7xx_hal_gpio.h"
#include "stdbool.h"
#ifndef SERIAL_H
#define SERIAL_H

static UART_HandleTypeDef huart1;
static bool serial_error = false;

void serial_err_handler(void);
void splitIm(void);

static char bliss[] = "/9j/4AAQSkZJRgABAQECWAJYAAD/4T7qRXhpZgAASUkqAAgAAAAKABoBBQABAAAAhgAAABsBBQABAAAAjgAAACgBAwABAAAAAg";

#endif
