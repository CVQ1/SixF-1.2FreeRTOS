#include "os_Task.h"
#include "usart1.h"
//#include "usart2.h"
#include "i2c1.h"
#include "servo.h"
#include "PCA_H_I2C_HAL.h"

osThreadId LEDThread1Handle, LEDThread2Handle;
osThreadId MessageprocessThreadHandle;
osThreadId Robot_ControlThreadHandle;

osTimerId myTimer01Handle;

osSemaphoreId R_ControlBinarySemHandle;

osMessageQId MessageQueue01Handle;

extern BTdata BTdata1;
uint32_t  A, B, C; //测试
MassageData01  data;//测试
uint32_t time=0;

static void LED_Thread1(void const *argument);
static void LED_Thread2(void const *argument);
static void Messageprocess_Thread(void const *argument);
static void Robot_Control_Thread(void const *argument);

void TimerCallback01(void const * argument);




void osMessagePutQueue01(MassageData01 data)
{
	MassageData01 *pdata = (MassageData01 *)malloc(sizeof(MassageData01));
	A = (uint32_t)pdata;////////////////////////测试
	pdata->flag = data.flag;
	pdata->variable = data.variable;
	osMessagePut(MessageQueue01Handle, (uint32_t)pdata, osWaitForever);
}

MassageData01  osMessageGetQueue01(void)
{
	MassageData01 BackData;
	osEvent event;
	uint32_t Data;
	
	event =osMessageGet(MessageQueue01Handle, osWaitForever);
	Data =0x20000000 | event.value.v;   //读到的数据莫名前16位是0,主动添加0x2000在前面
	B = Data;/////////////////////测试
	BackData.flag = ((MassageData01*)Data)->flag;
	BackData.variable = ((MassageData01*)Data)->variable;
	
	free((MassageData01*)Data); //一定要释放内存,不然GG
	return BackData;
	
}



void os_task_init(void)
{
	osMessageQDef(MessageQueue01, 16, uint16_t);
	MessageQueue01Handle = osMessageCreate(osMessageQ(MessageQueue01), NULL);
	
	osSemaphoreDef(R_ControlBinarySem);
	R_ControlBinarySemHandle = osSemaphoreCreate(osSemaphore(R_ControlBinarySem), 1);
	
	osTimerDef(myTimer01, TimerCallback01);
	myTimer01Handle = osTimerCreate(osTimer(myTimer01), osTimerPeriodic, NULL);
	
	//	osStatus timerresult = osOK ; 
//	timerresult = 
	osTimerStart(myTimer01Handle, 50); /* Start scheduler */  
	//	if(osOK == timerresult)  
	//	 osKernelStart();

	osThreadDef(LED1, LED_Thread1, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);

	osThreadDef(LED2, LED_Thread2, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
	
	osThreadDef(Messageprocess, Messageprocess_Thread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE)
		;
	osThreadDef(Robot_Control, Robot_Control_Thread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
  
	LEDThread1Handle = osThreadCreate(osThread(LED1), NULL);
 
	LEDThread2Handle = osThreadCreate(osThread(LED2), NULL);
	
	MessageprocessThreadHandle = osThreadCreate(osThread(Messageprocess), NULL);
	
	Robot_ControlThreadHandle = osThreadCreate(osThread(Robot_Control), NULL);
	
	/* Start scheduler */
	osKernelStart();
	
	

}

static void LED_Thread1(void const *argument)
{
	(void) argument;
	
	
	for (;;)
	{
			
			//	osThreadSuspend(LEDThread2Handle);
		
			//	osThreadResume(LEDThread2Handle);
//       a = osMessageGetQueue01();
//		
//		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
	   osDelay(2000);
		//osSemaphoreWait(R_ControlBinarySemHandle, osWaitForever);
//      OLED_ShowStr(0, 2, (unsigned char*)"Hello chen", 2);
//		osDelay(2000);
// 	OLED_ShowStr(0, 2, (unsigned char*)"Hello CVQ ", 2);
		
	}
}
static void LED_Thread2(void const *argument)
{
	uint32_t count;
	(void) argument;
  
	for (;;)
	{
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
		osDelay(500);
	}
}

void Messageprocess_Thread(void const *argument)
{
	for (;;)
	{
			
		MassageData01 data;
	
		data = osMessageGetQueue01();
		if (data.flag == tarmodeflag)
		{
			switch (data.variable)
			{
			case 'A':USART1_SetLED(2, 1); USART1_SetLED(3, 0);
				OLED_ShowStr(0, 0, (unsigned char*)"Tarmo: A", 2);
				break;
			case 'B':
				OLED_ShowStr(0, 0, (unsigned char*)"Tarmo: B", 2);
				break;
			case 'C':
				OLED_ShowStr(0, 0, (unsigned char*)"Tarmo: C", 2);
				break;
			case'F':USART1_SetLED(2, 0); USART1_SetLED(3, 1); 
				OLED_ShowStr(0, 0, (unsigned char*)"Tarmo: F", 2); 
				break;
			case'R':USART1_SetLED(2, 0); USART1_SetLED(3, 1); 
				OLED_ShowStr(0, 0, (unsigned char*)"Tarmo: R", 2); 
				break;
			case'L':USART1_SetLED(2, 0); USART1_SetLED(3, 1); 
				OLED_ShowStr(0, 0, (unsigned char*)"Tarmo: L", 2); 
				break;
			default:  break;
			}
		}
		else if (data.flag == modeflag)
		{
			switch (data.variable)
			{
			case 'A':USART1_SetLED(2, 1); USART1_SetLED(3, 0);
				OLED_ShowStr(0, 2, (unsigned char*)"mode : A", 2);
				break;
			case 'B':
				OLED_ShowStr(0, 2, (unsigned char*)"mode : B", 2);
				break;
			case 'C':
				OLED_ShowStr(0, 2, (unsigned char*)"mode : C", 2);
				break;
			case'F':USART1_SetLED(2, 0); USART1_SetLED(3, 1); 
				OLED_ShowStr(0, 2, (unsigned char*)"mode : F", 2); break;
			case'R':USART1_SetLED(2, 0); USART1_SetLED(3, 1); 
				OLED_ShowStr(0, 2, (unsigned char*)"mode : R", 2); break;
			case'L':USART1_SetLED(2, 0); USART1_SetLED(3, 1); 
				OLED_ShowStr(0, 2, (unsigned char*)"mode : L", 2); break;
			default:  break;
			}
		}
		else if (data.flag == stepflag)
		{
			OLED_ShowChar(80, 2, (unsigned char)(data.variable / 10 + 48), 2);
			OLED_ShowChar(90, 2, (unsigned char)(data.variable % 10 + 48), 2);
		}
		else if (data.flag == keyflag)
		{
			
				USART1_SetLED(2, data.variable);
			if (data.variable == 0)
			{
				OLED_ShowStr(0, 4, (unsigned char*)"unLock", 2);
			}
			else
			{
				OLED_ShowStr(0, 4, (unsigned char*)"Lock  ", 2);
			}
			
		}
		
		
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
	
	
	}
	
}

static void Robot_Control_Thread(void const *argument)
{
	for (;;)
	{
		osSemaphoreWait(R_ControlBinarySemHandle, osWaitForever);
		if ((BTdata1.Key & 0x10) == 0)
		{
			servo_PWMData();	
		}
	}
}
void TimerCallback01(void const * argument)
{
	/* USER CODE BEGIN Callback01 */
	//HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
	if(time < 50)
	{
		time++;
		
	}
	else if(time<52)
	{
		time++;
		PCA9685_Enable();
	}
	osSemaphoreRelease(R_ControlBinarySemHandle);
	//
//	MassageData01 data;
//	data.flag = tarmodeflag;
//	data.variable = 'A';
//	osMessagePutQueue01(data);
	/* USER CODE END Callback01 */
}