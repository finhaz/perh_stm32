#include <string.h> 
#include "24l01.h"
#include "encoder.h"
#include "stm32f10x.h"
#include "usart.h"
#include "pid.h"
//TIM2 cconnect to encoder,a-b
//TIM2_CH1 ----- PA0
//TIM2_CH2 ----- PA1

//���ֵ�����ٱ�64 �� 1�� תһȦ12����

void Encoder_Configuration(void)
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		TIM_ICInitTypeDef TIM_ICInitStructure;
		GPIO_InitTypeDef GPIO_InitStructure;

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);        // ENCODER_TIMERʱ�ӳ�ʼ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);        // ENCODER_GPIOʱ�ӳ�ʼ��

		GPIO_StructInit(&GPIO_InitStructure);        //TI1 TI2��ʼ��
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;        //��������
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		/*----------------------------------------------------------------*/        

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʹ�ܣԣɣ�2
		TIM_DeInit(TIM2);
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

		TIM_TimeBaseStructure.TIM_Period =0xffff; //
		TIM_TimeBaseStructure.TIM_Prescaler =0;        //����Ԥ��Ƶ��
		TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;        //����ʱ�ӷ�Ƶϵ��������Ƶ
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
		/*��ʼ��TIM2��ʱ�� */
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

		/*-----------------------------------------------------------------*/
		//�������� ����ģʽ
		TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, 
		TIM_ICPolarity_Rising, TIM_ICPolarity_Rising); //TIM_ICPolarity_Rising�����ز���
		TIM_ICStructInit(&TIM_ICInitStructure);
		TIM_ICInitStructure.TIM_ICFilter = 12; //�Ƚ��˲���
		TIM_ICInit(TIM2, &TIM_ICInitStructure);

		// Clear all pending interrupts
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);
		TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); //ʹ���ж�
		//Reset counter
		TIM2->CNT =0;

		TIM_Cmd(TIM2, ENABLE); //ʹ�ܶ�ʱ��2
}


EncoderType GetEncoder;

void Get_Encoder_ONE(void)
{
  s32 CNT2_temp,CNT2_last;
  
  GetEncoder.cnt2 = TIM2-> CNT;
  CNT2_last = GetEncoder.CNT2;
  CNT2_temp = GetEncoder.rcnt2 * prd + GetEncoder.cnt2;  
  GetEncoder.V2 = CNT2_temp - CNT2_last;		
  
  while ((s32)(GetEncoder.V2)>Vbreak)				 
  {							      
   GetEncoder.rcnt2--;					      
   CNT2_temp = GetEncoder.rcnt2 * prd + GetEncoder.cnt2;
   GetEncoder.V2 = CNT2_temp - CNT2_last;		 
  }							     
  while ((s32)(GetEncoder.V2)<-Vbreak)			   
  {							      
   GetEncoder.rcnt2++;					      
   CNT2_temp = GetEncoder.rcnt2 * prd + GetEncoder.cnt2;
   GetEncoder.V2 = CNT2_temp - CNT2_last;		 
  }
  GetEncoder.CNT2 = CNT2_temp;						 
  
	TIM2-> CNT=0;
	
}





