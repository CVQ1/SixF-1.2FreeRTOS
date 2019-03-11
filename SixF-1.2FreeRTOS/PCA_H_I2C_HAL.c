#include "PCA_H_I2C_HAL.h"
//
extern I2C_HandleTypeDef hi2c1;
//
//void I2C1_Init(void)
//{
//
//	hi2c1.Instance = I2C1;
//	hi2c1.Init.ClockSpeed = 100000;
//	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
//	hi2c1.Init.OwnAddress1 = 0;
//	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
//	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
//	hi2c1.Init.OwnAddress2 = 0;
//	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
//	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
//	if (HAL_I2C_Init(&hi2c1) != HAL_OK)
//	{
//		_Error_Handler(__FILE__, __LINE__);
//	}
//
//}

void PCA9685_Enable(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
}

void PCA9685_Disable(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);		
}

uint8_t PCA9685_read(uint8_t address, uint8_t addr)
	{
	//Send address to start reading from.
	uint8_t tx[1];
	uint8_t data;
		tx[0] = addr;
//		if (HAL_I2C_Master_Transmit(&hi2c1, address, tx, 1, 10000) != HAL_OK)
//		{
//			while (1)
//				;
//		}
//		
//		if (HAL_I2C_Master_Receive(&hi2c1, address, &data, 1, 10000) != HAL_OK)
//		{
//			while (1)
//				;
//		}
		
	return data;
}

void PCA9685_write(uint8_t address, uint8_t addr, uint8_t buffer) 
	{
	//Send address to start reading from.
	uint8_t tx[2];
	tx[0] = addr;
	tx[1] = buffer;
	
				
//	if (HAL_I2C_Master_Transmit(&hi2c1, address, tx, 2, 10000) != HAL_OK)
//	{
//		while (1)
//			;
//	}
				
}

void SetPWMFreq(uint8_t address,float freq)
{
	uint32_t prescale, oldmode, newmode;
	float prescaleval;
	freq *= 0.977;   // Correct for overshoot in the frequency setting (see issue #11).
	prescaleval = (uint8_t)(25000000 / (4096 * freq));

	prescale = (uint8_t)(prescaleval - 1 + 0.5);

	oldmode = PCA9685_read(address, PCA9685_MODE1);
	newmode = (oldmode & 0x7F) | 0x10;  // sleep
	PCA9685_write(address,PCA9685_MODE1, newmode);   // go to sleep
	PCA9685_write(address,PCA9685_PRESCALE, prescale);   // set the prescaler
	PCA9685_write(address,PCA9685_MODE1, oldmode);
	PCA9685_write(address,PCA9685_MODE1, oldmode | 0xa1);

}
void SetPWM(uint8_t address,uint32_t num, uint32_t on, uint32_t off)
{
	PCA9685_write(address, LED0_ON_L + 4*num, on);
	PCA9685_write(address, LED0_ON_H + 4*num, on >> 8);
	PCA9685_write(address, LED0_OFF_L + 4*num, off);
	PCA9685_write(address, LED0_OFF_H + 4*num, off >> 8);
}

void PCA9685_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	__HAL_RCC_GPIOB_CLK_ENABLE();
	  /*   
	  PB8    ------> PCA1
	  PB9     ------> PCA2 
	  */
	GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
	
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	PCA9685_Enable();
	
	
	
	PCA9685_write(PWM_Address_0, PCA9685_MODE1, 0x00);
	PCA9685_write(PWM_Address_1, PCA9685_MODE1, 0x00);

	SetPWMFreq(PWM_Address_1, 50); 
	SetPWMFreq(PWM_Address_0, 50); 
}		


