#pragma once
#ifndef _SERVO_H
#define _SERVO_H

//#include "_stdint.h"
#include "stm32f4xx_hal.h"

void Set_PWM(uint32_t num, uint32_t off);
void servo_PWMData(void);
void Calculate_Increment(void);



#endif
