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
		enum Flag flag;                             //Ҫ���µ�����
		uint8_t variable;							 //��ֵ
	}MassageData01;                                                           //��Ϣ�ṹ��
	
	extern osMessageQId MessageQueue01Handle;
	
	
	void os_task_init(void);
	void os_task_start(void);

	void osMessagePutQueue01(MassageData01 data);     //������Ϣ
	MassageData01  osMessageGetQueue01(void);            //�ȴ���Ϣ

#ifdef __cplusplus
}
#endif

#endif