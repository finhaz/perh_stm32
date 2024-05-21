#include "stm32f10x.h"
#include "misc.h"


void Timer_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	/*��ʼ��ΪĬ��ֵ*/
	TIM_DeInit(TIM2);
//	TIM_InternalClockConfig(TIM2);	
   	/* TIM2 clock enable */
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	/* TIM2����ʱ������������*/
	TIM_TimeBaseStructure.TIM_Period = 20;		               //����ֵ:18  ��ʱ0.18ms  19200  3.5���ַ�����
	TIM_TimeBaseStructure.TIM_Prescaler =720-1;    	               //Ԥ��Ƶ,����:720, 10us
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;  	           //ʱ�ӷ�Ƶ����Ϊ1
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;    //���ϼ���
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);	               // Time base configuration

	/*Ԥ����������ж�λ*/
	TIM_ARRPreloadConfig(TIM2, ENABLE);//ʹ��Ԥװ��
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);

//	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//TIM_SetCounter(TIM2, 0);

	/* ��������ж�*/
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); //��������ж�
	TIM_Cmd(TIM2,DISABLE); 

}
