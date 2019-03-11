#pragma once
#pragma once
#ifndef __II2C1__
#define __II2C1__
#ifdef __cplusplus
extern "C" {
#endif
	 
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "system_stm32f4xx.h"  

#define OLED_ADDRESS	0x78 //ͨ������0R����,������0x78��0x7A������ַ -- Ĭ��0x78
	
	void I2C1_Init(void);

	void I2C_WriteByte(uint8_t addr, uint8_t data);
	void WriteCmd(unsigned char I2C_Command);
	void WriteDat(unsigned char I2C_Data);
	void OLED_Init(void);
	void OLED_SetPos(unsigned char x, unsigned char y);
	void OLED_Fill(unsigned char fill_Data);
	void OLED_CLS(void);
	void OLED_ON(void);
	void OLED_OFF(void);
	void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);
	void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char ch, unsigned char TextSize);
	void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);
	void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[]);

	
#ifdef __cplusplus
}
#endif

#endif