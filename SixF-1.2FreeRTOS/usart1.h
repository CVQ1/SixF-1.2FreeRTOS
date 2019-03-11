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
#define USE_PRINTF

	typedef struct
	{
		
		int32_t A;
		int32_t B;
		int32_t C;
		int32_t D;
		int32_t E;
		int32_t F;
		int16_t P1;
		int16_t P2;
		int16_t P3;
		uint16_t Key;
	
	}BTdata;
	void USART1_UART_Init(void);
	void USART1_SetLED(uint8_t LED, uint8_t state);
	void USART1_SetCdata(uint8_t C, uint16_t Cdata);
		void UART1_Handler(void);

#ifdef __cplusplus
}
#endif

#endif
