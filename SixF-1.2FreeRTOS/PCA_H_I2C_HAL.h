#pragma once
#ifndef __PCA_H_I2C_HAL_H__
#define __PCA_H_I2C_HAL_H__

#ifdef __cplusplus
extern "C" {
#endif
	 
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "system_stm32f4xx.h"  

#define PCA9685_SUBADR1 0x2
#define PCA9685_SUBADR2 0x3
#define PCA9685_SUBADR3 0x4

#define PCA9685_MODE1 0x0
#define PCA9685_PRESCALE 0xFE

#define LED0_ON_L 0x6
#define LED0_ON_H 0x7
#define LED0_OFF_L 0x8
#define LED0_OFF_H 0x9

#define ALLLED_ON_L 0xFA
#define ALLLED_ON_H 0xFB
#define ALLLED_OFF_L 0xFC
#define ALLLED_OFF_H 0xFD

#define 	PWM_Address_0 	0x80
#define 	PWM_Address_1 	0x90
	
//	void I2C1_Init(void);
	void PCA9685_write(uint8_t address, uint8_t addr, uint8_t buffer); 
	uint8_t PCA9685_read(uint8_t address, uint8_t addr);
	void PCA9685_Enable(void);
	void PCA9685_Disable(void);
	void PCA9685_Init(void);
	void SetPWMFreq(uint8_t address, float freq); 
	void SetPWM(uint8_t address, uint32_t num, uint32_t on, uint32_t off);

#ifdef __cplusplus
}
#endif

#endif
