#include "stm32f10x.h"
#include "misc.h"


void Timer_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	/*初始化为默认值*/
	TIM_DeInit(TIM2);
//	TIM_InternalClockConfig(TIM2);	
   	/* TIM2 clock enable */
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	/* TIM2做定时器，基础设置*/
	TIM_TimeBaseStructure.TIM_Period = 20;		               //计数值:18  定时0.18ms  19200  3.5个字符长度
	TIM_TimeBaseStructure.TIM_Prescaler =720-1;    	               //预分频,除数:720, 10us
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;  	           //时钟分频因子为1
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;    //向上计数
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);	               // Time base configuration

	/*预先清除更新中断位*/
	TIM_ARRPreloadConfig(TIM2, ENABLE);//使能预装载
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);

//	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//TIM_SetCounter(TIM2, 0);

	/* 配置溢出中断*/
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); //允许更新中断
	TIM_Cmd(TIM2,DISABLE); 

}
