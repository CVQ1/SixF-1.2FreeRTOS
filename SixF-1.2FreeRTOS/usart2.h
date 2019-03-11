#pragma once
#ifndef __USART1_H__
#define __USART1_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "system_stm32f4xx.h"  
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
	/*#include <stdarg.h>*/

	void USART2_UART_Init(void);
	void USART2_SetLED(uint8_t LED, uint8_t state);
	
	void UART2_Handler(void);

#ifdef __cplusplus
}
#endif

#endif
