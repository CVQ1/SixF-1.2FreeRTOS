#include "usart2.h"



UART_HandleTypeDef huart2;
uint8_t transfmit2[10];


void USART2_UART_Init(void)
{


	huart2.Instance = USART1;
	huart2.Init.BaudRate = 9600;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart2) != HAL_OK)
	{
		//_Error_Handler(__FILE__, __LINE__);
	}



	//usart2_time = HAL_GetTick();
	//HAL_UART_Receive_IT(&huart2, receive2, 1);
	
}
void USART2_SetLED(uint8_t LED, uint8_t state)
{
	//	
	//	while (huart2.TxXferCount != 0)
	//		;
	
			transfmit2[0] = 0xA5;
	transfmit2[1] = 0x5A;
	transfmit2[2] = 0x05;
	transfmit2[3] = 0xE0 | LED;
	transfmit2[4] = state;
	transfmit2[5] = (uint8_t)(transfmit2[2] + transfmit2[3] + transfmit2[4]);
	transfmit2[6] = 0xAA;
			HAL_UART_Transmit_IT(&huart2, transfmit2, 7);
	
	
	//	HAL_UART_Transmit(&huart2, transfmit2, 2, 100);
}

void UART2_Handler(void)
{
	
}