#ifndef __PID_H__
#define __PID_H__
#include "stm32f10x.h"

void PID_Motor_Init(void);						//���õ��PID�����ĳ�ʼֵ
int32_t IncPIDCalc_Motor(s32 NextPoint);		//���PID����ó�����ֵ

void PID_Steer_Parameter_1(void);

#endif
