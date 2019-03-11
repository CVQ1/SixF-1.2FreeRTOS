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
	{  350, 380, 290, 205, 412, 315, 155, 420, 297, 215, 351, 304, 251, 358, 344, 378, 235, 318 },  // A:抬前  B:定原 -------启动部分           0
	{  325, 410, 280, 205, 412, 315, 155, 440, 297, 215, 351, 304, 165, 410, 361, 378, 235, 318 }, // A:落前  B:定原                                     1
	
	{  310, 407, 293, 240, 380, 280, 240, 398, 307, 215, 317, 304, 196, 395, 350, 280, 270, 340 }, // A:蹬1	B:抬前									  2
	{  300, 399, 307, 240, 380, 280, 251, 398, 309, 215, 317, 304, 213, 388, 329, 280, 270, 340 },  // A:蹬2	B:抬前									  3
	{  268, 403, 316, 240, 380, 280, 285, 395, 322, 215, 317, 304, 230, 380, 315, 280, 270, 340 },  // A:蹬3	B:抬前
	{  240, 410, 325, 168, 430, 260, 305, 399, 336, 215, 351, 304, 240, 377, 307, 295, 232, 375 },  // A:蹬4	B:落前 --------A后B前  
	
	{  350, 380, 290, 185, 425, 275, 310, 350, 307, 250, 338, 296, 280, 330, 344, 305, 235, 355 },   // A:抬后  B:蹬1--------A后B前			  6
	{  350, 380, 290, 200, 416, 288, 310, 350, 307, 270, 335, 290, 280, 330, 344, 317, 240, 338 },  // A:抬后  B:蹬2
	{  350, 380, 290, 210, 410, 300, 310, 350, 307, 288, 338, 280, 280, 330, 344, 340, 240, 327 },  // A:抬后  B:蹬3
	{  325, 410, 280, 205, 412, 315, 155, 440, 297, 302, 345, 263, 165, 410, 361, 378, 235, 318 },  // A:落前  B:蹬4 --------A前B后
	
	{  310, 407, 293, 240, 380, 280, 240, 398, 307, 315, 305, 285, 196, 395, 350, 280, 270, 340 },  // A:蹬1	B:抬后 --------A前B后			  10
	{  300, 399, 307, 240, 380, 280, 251, 398, 309, 315, 305, 285, 213, 388, 329, 280, 270, 340 },  // A:蹬2	B:抬后
	{  268, 403, 316, 240, 380, 280, 285, 395, 322, 315, 305, 285, 230, 380, 315, 280, 270, 340 },  // A:蹬3	B:抬后
	{  240, 410, 325, 168, 430, 260, 305, 399, 336, 215, 351, 304, 240, 377, 307, 295, 232, 375 },  // A:蹬4	B:落前 --------A后B前  
	
	{  310, 360, 325, 185, 425, 275, 310, 350, 307, 250, 338, 296, 295, 330, 307, 305, 235, 355 },  // A:3抬后,余抬原  B:蹬1--------A后B前  14
	{  310, 360, 325, 200, 416, 288, 310, 350, 307, 270, 335, 290, 295, 330, 307, 317, 240, 338 },  // A:3抬后,余抬原  B:蹬2
	{  310, 360, 325, 210, 410, 300, 310, 350, 307, 288, 338, 280, 295, 330, 307, 340, 240, 327 },  // A:3抬后,余抬原  B:蹬3
	{  240, 410, 325, 205, 412, 315, 240, 398, 307, 302, 345, 263, 240, 377, 307, 378, 235, 318 },  // A:落原				  B:蹬4 --------除3外全归原
	
	{ 240, 410, 325, 205, 412, 315, 240, 398, 307, 315, 305, 285, 240, 377, 307, 378, 235, 318 },    // A:定	B:4抬后									   18
	{ 240, 410, 325, 205, 412, 315, 240, 398, 307, 215, 351, 304, 240, 377, 307, 378, 235, 318 },    // A:定	B:4落原
	
	{  310, 407, 293, 270, 365, 315, 240, 398, 307, 315, 305, 285, 196, 395, 350, 310, 275, 318 },  // A:蹬1	B:4抬后,余抬原 --------A前B后 20
	{  300, 399, 307, 270, 365, 315, 251, 398, 309, 315, 305, 285, 213, 388, 329, 310, 275, 318 },  // A:蹬2	B:4抬后,余抬原
	{  268, 403, 316, 270, 365, 315, 285, 395, 322, 315, 305, 285, 230, 380, 315, 310, 275, 318 },  // A:蹬3	B:4抬后,余抬原
	{  240, 410, 325, 205, 412, 315, 305, 399, 336, 215, 351, 304, 240, 377, 307, 378, 235, 318 },  // A:蹬4	B:落原 --------除4外全归原
	
	{  240, 410, 325, 205, 412, 315, 310, 350, 307, 215, 351, 304, 240, 377, 307, 378, 235, 318 },  // A:3抬后	B:定										24
	{  240, 410, 325, 205, 412, 315, 240, 398, 307, 215, 351, 304, 240, 377, 307, 378, 235, 318 }   // A:3落原	B:定
	
};
uint16_t pwm[18], tarpwm[18],lpwm[18],ltarpwm[18]; //pwm值 0--4096  int
float incpwm[18];
struct 
{
	uint8_t countflag;  //细分完成标志   0(完成) 1--20
	uint8_t modeflag;  //运动模式标志   A B C D
	uint8_t tarmodeflag;//目标运动模式
	uint8_t stepflag;    //步骤执行记录    0 1 2 3 4 5
}pwmflag;

void Robot_Stand(void)  //站立--------------A
{
	uint8_t i;
	for (i = 0; i < 18; i++)
	{
		tarpwm[i] = stand[i];
	}
	pwmflag.stepflag = 1;
}
void Robot_Down(void)  //蹲--------------C
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

void Robot_Forward(void) //前进------------F
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
void Robot_Control(void)     //运动模式更新判断函数
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
	if (off > 512)         off = 512;     //限制0.5ms--2.5ms   满值4096
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

void Calculate_Increment(void)								//舵机转动增量计算函数--已更新过tarpwm后
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
void servo_PWMData(void)    //执行PWM数据,,需要定时循环调用
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
			
			Robot_Control();     //运动模式更新判断函数
			
			Calculate_Increment();   //舵机转动增量计算函数--已更新过tarpwm后
		}
	
	
}