#include <math.h>
#include "os_Task.h"
#include "servo.h"
#include "PCA_H_I2C_HAL.h"

const uint16_t stand[18] = { 240, 410, 325, 205, 412, 315, 240, 398, 307, 215, 351, 304, 240, 377, 307, 378, 235, 318 };
const uint16_t test[7][18] = {
	{ 155, 420, 297, 350, 380, 290, 251, 358, 344, 307, 307, 307, 307, 307, 307, 307, 307, 307 },
    {155, 440, 290, 325, 410, 280, 165, 410, 361, 200, 200, 200, 200, 200, 200, 200, 200, 200, },
	{240, 398, 307, 310, 407, 293, 196, 395, 350, 200, 200, 200, 200, 200, 200, 200, 200, 200, },
	{251, 398, 309, 300, 399, 307, 213, 388, 329, 200, 200, 200, 200, 200, 200, 200, 200, 200, },
	{285, 395, 322, 268, 403, 316, 230, 380, 315, 200, 200, 200, 200, 200, 200, 200, 200, 200, },
	{305,399, 336, 240, 410, 325, 240, 377, 307, 200, 200, 200, 200, 200, 200, 200, 200, 200, },
	{350, 360, 320, 240, 410, 325, 240, 377, 307, 200, 200, 200, 200, 200, 200, 200, 200, 200, }
};
const uint16_t down[18] = { 210, 360, 325, 270, 365, 315, 290, 355, 307, 270, 305, 304, 295, 330, 307, 310, 275, 318 };
const uint16_t Forward[26][18] = {
//     1					  2						  3						4						5					   6			
	{  350, 380, 290, 205, 412, 315, 155, 420, 297, 215, 351, 304, 251, 358, 344, 378, 235, 318 },  // A:̧ǰ  B:��ԭ -------��������           0
	{  325, 410, 280, 205, 412, 315, 155, 440, 297, 215, 351, 304, 165, 410, 361, 378, 235, 318 }, // A:��ǰ  B:��ԭ                                     1
	
	{  310, 407, 293, 240, 380, 280, 240, 398, 307, 215, 317, 304, 196, 395, 350, 280, 270, 340 }, // A:��1	B:̧ǰ									  2
	{  300, 399, 307, 240, 380, 280, 251, 398, 309, 215, 317, 304, 213, 388, 329, 280, 270, 340 },  // A:��2	B:̧ǰ									  3
	{  268, 403, 316, 240, 380, 280, 285, 395, 322, 215, 317, 304, 230, 380, 315, 280, 270, 340 },  // A:��3	B:̧ǰ
	{  240, 410, 325, 168, 430, 260, 305, 399, 336, 215, 351, 304, 240, 377, 307, 295, 232, 375 },  // A:��4	B:��ǰ --------A��Bǰ  
	
	{  350, 380, 290, 185, 425, 275, 310, 350, 307, 250, 338, 296, 280, 330, 344, 305, 235, 355 },   // A:̧��  B:��1--------A��Bǰ			  6
	{  350, 380, 290, 200, 416, 288, 310, 350, 307, 270, 335, 290, 280, 330, 344, 317, 240, 338 },  // A:̧��  B:��2
	{  350, 380, 290, 210, 410, 300, 310, 350, 307, 288, 338, 280, 280, 330, 344, 340, 240, 327 },  // A:̧��  B:��3
	{  325, 410, 280, 205, 412, 315, 155, 440, 297, 302, 345, 263, 165, 410, 361, 378, 235, 318 },  // A:��ǰ  B:��4 --------AǰB��
	
	{  310, 407, 293, 240, 380, 280, 240, 398, 307, 315, 305, 285, 196, 395, 350, 280, 270, 340 },  // A:��1	B:̧�� --------AǰB��			  10
	{  300, 399, 307, 240, 380, 280, 251, 398, 309, 315, 305, 285, 213, 388, 329, 280, 270, 340 },  // A:��2	B:̧��
	{  268, 403, 316, 240, 380, 280, 285, 395, 322, 315, 305, 285, 230, 380, 315, 280, 270, 340 },  // A:��3	B:̧��
	{  240, 410, 325, 168, 430, 260, 305, 399, 336, 215, 351, 304, 240, 377, 307, 295, 232, 375 },  // A:��4	B:��ǰ --------A��Bǰ  
	
	{  310, 360, 325, 185, 425, 275, 310, 350, 307, 250, 338, 296, 295, 330, 307, 305, 235, 355 },  // A:3̧��,��̧ԭ  B:��1--------A��Bǰ  14
	{  310, 360, 325, 200, 416, 288, 310, 350, 307, 270, 335, 290, 295, 330, 307, 317, 240, 338 },  // A:3̧��,��̧ԭ  B:��2
	{  310, 360, 325, 210, 410, 300, 310, 350, 307, 288, 338, 280, 295, 330, 307, 340, 240, 327 },  // A:3̧��,��̧ԭ  B:��3
	{  240, 410, 325, 205, 412, 315, 240, 398, 307, 302, 345, 263, 240, 377, 307, 378, 235, 318 },  // A:��ԭ				  B:��4 --------��3��ȫ��ԭ
	
	{ 240, 410, 325, 205, 412, 315, 240, 398, 307, 315, 305, 285, 240, 377, 307, 378, 235, 318 },    // A:��	B:4̧��									   18
	{ 240, 410, 325, 205, 412, 315, 240, 398, 307, 215, 351, 304, 240, 377, 307, 378, 235, 318 },    // A:��	B:4��ԭ
	
	{  310, 407, 293, 270, 365, 315, 240, 398, 307, 315, 305, 285, 196, 395, 350, 310, 275, 318 },  // A:��1	B:4̧��,��̧ԭ --------AǰB�� 20
	{  300, 399, 307, 270, 365, 315, 251, 398, 309, 315, 305, 285, 213, 388, 329, 310, 275, 318 },  // A:��2	B:4̧��,��̧ԭ
	{  268, 403, 316, 270, 365, 315, 285, 395, 322, 315, 305, 285, 230, 380, 315, 310, 275, 318 },  // A:��3	B:4̧��,��̧ԭ
	{  240, 410, 325, 205, 412, 315, 305, 399, 336, 215, 351, 304, 240, 377, 307, 378, 235, 318 },  // A:��4	B:��ԭ --------��4��ȫ��ԭ
	
	{  240, 410, 325, 205, 412, 315, 310, 350, 307, 215, 351, 304, 240, 377, 307, 378, 235, 318 },  // A:3̧��	B:��										24
	{  240, 410, 325, 205, 412, 315, 240, 398, 307, 215, 351, 304, 240, 377, 307, 378, 235, 318 }   // A:3��ԭ	B:��
	
};
uint16_t pwm[18], tarpwm[18],lpwm[18],ltarpwm[18]; //pwmֵ 0--4096  int
float incpwm[18];
struct 
{
	uint8_t countflag;  //ϸ����ɱ�־   0(���) 1--20
	uint8_t modeflag;  //�˶�ģʽ��־   A B C D
	uint8_t tarmodeflag;//Ŀ���˶�ģʽ
	uint8_t stepflag;    //����ִ�м�¼    0 1 2 3 4 5
}pwmflag;

void Robot_Stand(void)  //վ��--------------A
{
	uint8_t i;
	for (i = 0; i < 18; i++)
	{
		tarpwm[i] = stand[i];
	}
	pwmflag.stepflag = 1;
}
void Robot_Down(void)  //��--------------C
{
	uint8_t i;
	for (i = 0; i < 18; i++)
	{
		tarpwm[i] = down[i];
	}
	pwmflag.stepflag = 0;
}
void Robot_Test(void) //---------------B
{
	uint8_t i;
	
	for (i = 0; i < 18; i++)
	{
		tarpwm[i] = test[pwmflag.stepflag][i];
	}
	pwmflag.stepflag++;
	if (pwmflag.stepflag >= 7)
	{
		pwmflag.stepflag = 0;
	}
}

void Robot_Forward(void) //ǰ��------------F
{
	uint8_t i;
	for (i = 0; i < 18; i++)
	{
		tarpwm[i] = Forward[pwmflag.stepflag][i];
	}
	if (pwmflag.tarmodeflag != 'F')
	{
		if (pwmflag.stepflag == 9)
		{
			pwmflag.stepflag = 20;
			return;
		}
		else if (pwmflag.stepflag == 13)
		{
			pwmflag.stepflag = 14;
			return;
		}
	}
	else if(pwmflag.stepflag == 13)
	{
		pwmflag.stepflag = 6;
		return;
	}
	
	pwmflag.stepflag++;
	if (pwmflag.stepflag >= 26 || pwmflag.stepflag==20)
	{
		pwmflag.stepflag = 0;
	}
	
}
void Robot_Control(void)     //�˶�ģʽ�����жϺ���
{
	MassageData01 data;
	if ((pwmflag.tarmodeflag != pwmflag.modeflag)&&(pwmflag.stepflag == 0 || pwmflag.modeflag == 'A'))
	{
		
		if (pwmflag.modeflag == 'A')
		{
			pwmflag.modeflag = pwmflag.tarmodeflag;
			
		}
		else
		{
			pwmflag.modeflag = 'A';
			
		}
		data.flag = modeflag;
		data.variable = pwmflag.modeflag;
		osMessagePutQueue01(data);
		
		pwmflag.stepflag = 0;                  /////////
	}
	else if(pwmflag.modeflag=='A'&&pwmflag.stepflag != 0)
	{
		return;
	}
	switch (pwmflag.modeflag)
	{
	case 'A' :Robot_Stand();   break;
	case 'B' :Robot_Test();      break;
	case 'C' :Robot_Down(); break;
	case 'F' : Robot_Forward(); break;
		default:pwmflag.tarmodeflag = 'A'; break;
	}
	data.flag = stepflag;
	data.variable = pwmflag.stepflag;
	osMessagePutQueue01(data);
}
void Set_PWM(uint32_t num, uint32_t off)
{
	if (off > 512)         off = 512;     //����0.5ms--2.5ms   ��ֵ4096
	else if (off < 102)  off = 102;
	
//	switch (num)
//	{
//	case 'A' :Robot_Stand(); break;
//	case 'B' :Robot_Test(); break;
//	case 'C' :Robot_Down(); break;
//	case 'F' : Robot_Forward(); break;
//	default:pwmflag.tarmodeflag = 'A'; break;
//	}
	
	if (num < 3)
	{
		SetPWM(PWM_Address_0, num, 0, off);
	}
	else if(num < 6)
	{
		SetPWM(PWM_Address_0, num+1, 0, off);
	}
	else if(num < 9)
	{
		SetPWM(PWM_Address_0, num+2, 0, off);
	}
	else if(num < 12)
	{
		SetPWM(PWM_Address_0, num+3, 0, off);
	}
	else if(num < 15)
	{
		SetPWM(PWM_Address_1, num-12, 0, off);
	}
	else if(num < 18)
	{
		SetPWM(PWM_Address_1, num-11, 0, off);
	}
	
//	if (num < 16)
//	{
//		
//	SetPWM(PWM_Address_0, num, 0, off);
//	}
//	else if(num < 18)
//	{
//		num = num - 16;
//	SetPWM(PWM_Address_1, num, 0, off);
//	}
	
}

void Calculate_Increment(void)								//���ת���������㺯��--�Ѹ��¹�tarpwm��
{	
	uint8_t i;
	for (i = 0; i < 18; i++)
	{
		incpwm[i] = (tarpwm[i] - pwm[i]) / 10.0;//////////////////////
		lpwm[i] = pwm[i];
		ltarpwm[i] = tarpwm[i];
	}
	pwmflag.countflag = 0;
}
void servo_PWMData(void)    //ִ��PWM����,,��Ҫ��ʱѭ������
{
	uint8_t i;
			
	if (pwmflag.countflag < 9)////////////////////
		{
			pwmflag.countflag++;
			for (i = 0; i < 18; i++)
			{
				if (pwm[i] != ltarpwm[i])
				{
					pwm[i] = (uint16_t)(lpwm[i] + (pwmflag.countflag + 1)*incpwm[i]);
					Set_PWM(i, pwm[i]);
				}
			}
		}
		else
		{	
			for (i = 0; i < 18; i++)
			{
				incpwm[i] = 0;
				if (pwm[i] != ltarpwm[i])
				{
					pwm[i] = ltarpwm[i];
					
					Set_PWM(i, pwm[i]);
				}
			}	
			
			Robot_Control();     //�˶�ģʽ�����жϺ���
			
			Calculate_Increment();   //���ת���������㺯��--�Ѹ��¹�tarpwm��
		}
	
	
}