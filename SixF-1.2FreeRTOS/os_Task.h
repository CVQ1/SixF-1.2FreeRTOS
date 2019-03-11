#pragma once
#ifndef __OS_TASK_H__
#define __OS_TASK_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stm32f4xx_hal.h>
#include <malloc.h>
#include <../CMSIS_RTOS/cmsis_os.h>

	enum Flag
	{
		modeflag,
		tarmodeflag,
		stepflag,
		keyflag,
	};
	typedef struct Data
	{
		enum Flag flag;                             //要更新的数据
		uint8_t variable;							 //数值
	}MassageData01;                                                           //消息结构体
	
	extern osMessageQId MessageQueue01Handle;
	
	
	void os_task_init(void);
	void os_task_start(void);

	void osMessagePutQueue01(MassageData01 data);     //发送消息
	MassageData01  osMessageGetQueue01(void);            //等待消息

#ifdef __cplusplus
}
#endif

#endif