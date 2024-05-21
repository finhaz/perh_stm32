#include "stm32f10x.h"

FlagStatus zheng_fan = RESET;//��ʼ����Ϊ��ת
void RCC_Config(void);
void TIM3_PWMShiftInit(void);
void BLDC_GPIOConfig(void);
void BLDC_TIM1Config(void);
void NVIC_Configuration(void);

int main()
{
	RCC_Config();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
//	NVIC_Configuration();
//	TIM3_PWMShiftInit();
	BLDC_GPIOConfig();
	BLDC_TIM1Config();
	while(1)
	{
	}
}


void RCC_Config(void)
{
	SystemInit();

	/* ʹ��APB2����ʱ��*/
	RCC_APB2PeriphClockCmd(   RCC_APB2Periph_GPIOA 
							| RCC_APB2Periph_GPIOB
			 	 	 	 	| RCC_APB2Periph_GPIOC 
							| RCC_APB2Periph_AFIO
							| RCC_APB2Periph_TIM1
 							, ENABLE );
	/*ʹ��APB1����ʱ��*/
 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3
	|RCC_APB1Periph_TIM4
							, ENABLE);	
	
	SysTick_Config(SystemCoreClock/500);//����ֵ=ʱ��Ƶ��(Hz)x��ʱʱ��(s)
}

void TIM3_PWMShiftInit(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	GPIO_InitTypeDef  GPIO_InitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
 
	
	/**********************TIM3 GPIO??*****************************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
 
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
 
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	/**********************???TimBase???*************************/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period =  3;  //??????????,Frequence = 36000000/PSC/(ARR+1);
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7199;     
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	
	/**********************???TIM3 OC???*************************/
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_Toggle;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 0;   //?????,CCR?CNT??????
 
	TIM_OC2Init(TIM3,&TIM_OCInitStruct);
	
	/**********************???TIM3 OC???*************************/
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_Toggle;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 1;   //?????,CCR?CNT??????
 
	TIM_OC3Init(TIM3,&TIM_OCInitStruct);
	
  TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;   
  TIM_OCInitStruct.TIM_Pulse = 2;   //???? = 180*CCR/(ARR+1)
	TIM_OC4Init(TIM3,&TIM_OCInitStruct);
 
	TIM_Cmd(TIM3, ENABLE);
}


/**********************************************************************
* Description    : �Զ�ʱ��1�Ͷ�ʱ��3��GPIO����
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
**********************************************************************/
void BLDC_GPIOConfig(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;  //TIM1���
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;						   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
			 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;//TIM1���
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;						   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);	   
}
/**********************************************************************
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
**********************************************************************/
void BLDC_TIM1Config(void)
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;			   //�����ṹ���������
		TIM_OCInitTypeDef  		TIM_OCInitStructure;               //����ṹ���������
		TIM_BDTRInitTypeDef  	TIM_BDTRInitStructure;			   //����ɲ���ṹ���������

		TIM_DeInit(TIM1);

		TIM_TimeBaseStructure.TIM_Prescaler = 7199;					   //TIM������ʼ��
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up ;//����������ģʽ,����Ƚϱ�־λֻ���ڱȽ������ϼ��㱻����
		TIM_TimeBaseStructure.TIM_Period = 3;					   //PWM 16K
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;				   

		TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);

		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle; 		   //TIM���ͨ����ʼ��
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
		TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;                  
		TIM_OCInitStructure.TIM_Pulse =0; 
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
		TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;         
		TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;//TIM_OCIdleState_Set;
		TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;          

		TIM_OC1Init(TIM1,&TIM_OCInitStructure); 

		TIM_OCInitStructure.TIM_Pulse =1;
		TIM_OC2Init(TIM1,&TIM_OCInitStructure);

		TIM_OCInitStructure.TIM_Pulse =2;
		TIM_OC3Init(TIM1,&TIM_OCInitStructure);

		TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;	//����ɲ����ʼ��
		TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
		TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF; 
		TIM_BDTRInitStructure.TIM_DeadTime = 100;
		TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;	 //��򿪣������������״̬���ң�������
		TIM_BDTRInitStructure.TIM_BreakPolarity =  TIM_BreakPolarity_Low ;
		TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;

		TIM_BDTRConfig(TIM1,&TIM_BDTRInitStructure);


		TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);  //ʹ�ܲ���ȽϼĴ���Ԥװ�أ�ͨ��1��

		TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);	 //ʹ�ܲ���ȽϼĴ���Ԥװ�أ�ͨ��2��

		TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable);	 //ʹ�ܲ���ȽϼĴ���Ԥװ�أ�ͨ��3��


		
		TIM_ClearITPendingBit( TIM1, TIM_IT_COM);
		TIM_ITConfig(TIM1, TIM_IT_COM ,ENABLE);

		TIM_CtrlPWMOutputs(TIM1,ENABLE);

		TIM_Cmd(TIM1,ENABLE);
}

/**********************************************************************
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
**********************************************************************/
void huanxiang(u8 irq_zhuanzi)
{



   if(zheng_fan==RESET)
  
  {									 //��ת
    switch(irq_zhuanzi)				 //����ת��λ�ã�����CCER�����λ��ת����ƫ����
    {
	  case 0x06:{	                      												
			  	  TIM1->CCER=0x3081;//1,4    ba
			    };break;
	  case 0x02:{
				  TIM1->CCER=0x3180;	//4,5	   bc       	   
	            };break;							
  	case 0x03:{
				  TIM1->CCER=0x3108;	//2,5    ac
			    };break;
	  case 0x01:{
				  TIM1->CCER=0x3018;	//2,3	    ab      		
	            };break;
	  case 0x05:{
				  TIM1->CCER=0x3810;	//3,6	    cb      		
	            };break;
	  case 0x04:{
	             			
				  TIM1->CCER=0x3801;    //1,6	  ca        		
	            };break;
				default:break;



}	  }
 else
  {									 //��ת
	switch(irq_zhuanzi)				 //����ת��λ�ã�����CCER�����λ��ת����ƫ����
    {
    //*
	   case 0x02:{										
			    TIM1->CCER=0x3810;//3,6        cb
			    };break;
	  case 0x06:{
				  TIM1->CCER=0x3018;//2,3		 ab         	   
	            };break;							
 	  case 0x04:{
				  TIM1->CCER=0x3108;//2,5    ac
			    };break;
	  case 0x05:{
				  TIM1->CCER=0x3180;//4,5		  bc        		
	            };break;
	  case 0x01:{
				  TIM1->CCER=0x3081;//1,4		  ba        		
	            };break;
	  case 0x03:{ 	              			
				  TIM1->CCER=0x3801;//1,6		  ca        		
	            };break;
	  default:break;
	  //*/
	  	
    }
  }
 
}


/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval : None
  */
void TIM1_TRG_COM_IRQHandler(void)
{ 
  TIM1->SR&=0;           //���жϱ�־
  
  huanxiang(6);		     //���û�����
}

/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the TIM1 Interrupt */
#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_TRG_COM_TIM17_IRQn;
#else
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_TRG_COM_IRQn;
#endif
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 

}
