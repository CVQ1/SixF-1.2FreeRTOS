#include "usart1.h"
#include "os_Task.h"
extern uint16_t tarpwm[18];

UART_HandleTypeDef huart1;
uint8_t receive1[24], transfmit1[10], receive1_flag;
uint32_t usart1_time = 0;
BTdata BTdata1;


extern struct 
{
	uint8_t countflag;   //细分完成标志   0(完成) 1--20
	uint8_t modeflag;   //运动模式标志   A B C D
	uint8_t tarmodeflag; //目标运动模式
	uint8_t stepflag;     //步骤执行记录    0 1 2 3 4 5
}pwmflag;

/**
* @brief 串口2初始化
* @param 无
* @note 无
* @retval 无
*/
void USART1_UART_Init(void)
{


		huart1.Instance = USART1;
		huart1.Init.BaudRate = 9600;
		huart1.Init.WordLength = UART_WORDLENGTH_8B;
		huart1.Init.StopBits = UART_STOPBITS_1;
		huart1.Init.Parity = UART_PARITY_NONE;
		huart1.Init.Mode = UART_MODE_TX_RX;
		huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
		huart1.Init.OverSampling = UART_OVERSAMPLING_16;
		if (HAL_UART_Init(&huart1) != HAL_OK)
		{
			//_Error_Handler(__FILE__, __LINE__);
		}



	//usart2_time = HAL_GetTick();
	HAL_UART_Receive_IT(&huart1, receive1, 1);
	
}
 
void USART1_SetLED(uint8_t LED, uint8_t state)
{
//	
//	while (huart1.TxXferCount != 0)
//		;
	
		transfmit1[0] = 0xA5;
		transfmit1[1] = 0x5A;
		transfmit1[2] = 0x05;
		transfmit1[3] = 0xE0 | LED;
		transfmit1[4] = state;
		transfmit1[5] = (uint8_t)(transfmit1[2] + transfmit1[3] + transfmit1[4]);
		transfmit1[6] = 0xAA;
//		HAL_UART_Transmit_IT(&huart1, transfmit1, 7);
	
	
//	HAL_UART_Transmit(&huart1, transfmit1, 2, 100);              //玄学问题,能接受,但不能发送
}
void USART1_SetCdata(uint8_t C, uint16_t Cdata)//C第几个参数,Cdata 参数值
{
	
//	transfmit1[0] = 0xA5;                        
//	transfmit1[1] = 0x5A;
//	transfmit1[2] = 0x06;
//	transfmit1[3] = 0xC0 | C;
//	transfmit1[4] = (uint8_t)Cdata>>8;
//	transfmit1[5] = (uint8_t)Cdata;
//	transfmit1[6] = (uint8_t)(transfmit1[2] + transfmit1[3] + transfmit1[4] );
//	transfmit1[7] = 0xAA;
//	
//	HAL_UART_Transmit_IT(&huart1, transfmit1, 8); 
}

void UART1_Handler(void)
{
	MassageData01 data;
	
	if (receive1[0] == 0xA5)
	{
		
		if (receive1[1] == 0x5A)
		{	
			
			if ((receive1[(uint8_t)(receive1[2]) + 1] == 0xAA) || (receive1[17])==0x34)
			{
				receive1[0] = 0x00;
				receive1[1] = 0x00;
				receive1[(uint8_t)(receive1[2]) + 1] = 0x00;
				switch (receive1[3])
				{
				case 0xF5: 
					BTdata1.A = (short)(receive1[4] << 8 | receive1[5]);
					BTdata1.B = (short)(receive1[6] << 8 | receive1[7]);
					BTdata1.C = (short)(receive1[8] << 8 | receive1[9]);
					BTdata1.D = (short)(receive1[10] << 8 | receive1[11]);
					BTdata1.E = (short)(receive1[12] << 8 | receive1[13]);
					BTdata1.F = (short)(receive1[14] << 8 | receive1[15]);
					break;		
				case 0xF1: 
					BTdata1.P1 = receive1[4];
					tarpwm[3] = 200+BTdata1.P1;
					break;
				case 0xF2: 
					BTdata1.P2 = receive1[4];
					tarpwm[4] =200+ BTdata1.P2;
					break;
				case 0xF3: 
					BTdata1.P3 = receive1[4];
					tarpwm[5] = 200+BTdata1.P3;
					break;
				case 0xB1: 
					pwmflag.tarmodeflag = 'A';
					USART1_SetLED(1, 1);
					
					data.flag = tarmodeflag;
					data.variable = 'A';
					osMessagePutQueue01(data);
					
					break;
				case 0xB2: 
					pwmflag.tarmodeflag = 'B';
					data.flag = tarmodeflag;
					data.variable = 'B';
					osMessagePutQueue01(data);
					break;
				case 0xB3: 
					pwmflag.tarmodeflag = 'C';
					data.flag = tarmodeflag;
					data.variable = 'C';
					osMessagePutQueue01(data);
					break;
				case 0xB4: 
					pwmflag.tarmodeflag = 'F';
					
					data.flag = tarmodeflag;
					data.variable = 'F';
					osMessagePutQueue01(data);
					break;
				case 0xB5: 
					if (BTdata1.Key & 0x10)
					{
						BTdata1.Key &= 0xffef;
						
						data.flag = keyflag;
						data.variable = 0;
						osMessagePutQueue01(data);
					}
					else
					{
						BTdata1.Key |= 0x10;
						
						data.flag = keyflag;
						data.variable = 1;
						osMessagePutQueue01(data);
					}
					break;
				case 0xB7: 
					pwmflag.tarmodeflag = 'R';
					
					data.flag = tarmodeflag;
					data.variable = 'R';
					osMessagePutQueue01(data);
					break;
					
				case 0xB9: 
					pwmflag.tarmodeflag = 'L';
					
					data.flag = tarmodeflag;
					data.variable = 'L';
					osMessagePutQueue01(data);
					break;
				default:  break;
				}
				receive1[3] = 0x00;
				receive1[17] = 0x00;
				HAL_UART_Receive_IT(&huart1, receive1, 1);
			}
			else
			{
				HAL_UART_Receive_IT(&huart1, &receive1[4], (uint8_t)receive1[2] - 2);
			//	HAL_UART_Receive_IT(&huart1, receive1, 1);
			}
		}
		else
		{
			HAL_UART_Receive_IT(&huart1, &receive1[1], 3);
			//HAL_UART_Receive_IT(&huart1, receive1, 1);HAL_UART_Transmit(&huart1, uint8_t *pData, uint16_t Size, uint32_t Timeout);
		}
	}
	else
	{
		HAL_UART_Receive_IT(&huart1, receive1, 1);
	}
}
