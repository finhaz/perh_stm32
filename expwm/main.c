#include "stm32f10x.h"


void RCC_Config(void);
void TIM1_PWMInit(void);
void TIM2_PWMInit(void);
void TIM3_PWMInit(void);


int main()
{
	RCC_Config();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	TIM1_PWMInit();
	TIM2_PWMInit();
	TIM3_PWMInit();
	while(1)
	{
	}
}


void RCC_Config(void)
{
	SystemInit();

	/* 使能APB2外设时钟*/
	RCC_APB2PeriphClockCmd(   RCC_APB2Periph_GPIOA 
							| RCC_APB2Periph_GPIOB
			 	 	 	 	| RCC_APB2Periph_GPIOC 
							| RCC_APB2Periph_AFIO
							| RCC_APB2Periph_TIM1
 							, ENABLE );
	/*使能APB1外设时钟*/
 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3
	|RCC_APB1Periph_TIM4
							, ENABLE);	
	
	SysTick_Config(SystemCoreClock/500);//重载值=时钟频率(Hz)x延时时间(s)
}

void TIM3_PWMInit(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	GPIO_InitTypeDef  GPIO_InitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/**********************TIM3 GPIO??*****************************/
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
 
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	/**********************???TimBase???*************************/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_CenterAligned1;
	TIM_TimeBaseInitStruct.TIM_Period =  7199;  
	TIM_TimeBaseInitStruct.TIM_Prescaler = 0;     
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	/**********************???TIM3 OC???*************************/
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 200;   
 
	TIM_OC3Init(TIM3,&TIM_OCInitStruct);
	TIM_OC4Init(TIM3,&TIM_OCInitStruct);
 
	TIM_Cmd(TIM3, ENABLE);
	
	  /* Enable the Update Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
}


void TIM2_PWMInit(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	GPIO_InitTypeDef  GPIO_InitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	/**********************TIM2 GPIO??*****************************/
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
 
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	
	/**********************???TimBase???*************************/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_CenterAligned1;
	TIM_TimeBaseInitStruct.TIM_Period =  7199;  
	TIM_TimeBaseInitStruct.TIM_Prescaler = 0;     
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	/**********************???TIM3 OC???*************************/
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStruct.TIM_OCMode =  TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 300;   //?????,CCR?CNT??????
 
	TIM_OC2Init(TIM2,&TIM_OCInitStruct);
	TIM_OC3Init(TIM2,&TIM_OCInitStruct);
 
//	TIM_Cmd(TIM2, ENABLE);
	TIM_Cmd(TIM2, DISABLE);
		  /* Enable the Update Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}



void TIM1_PWMInit(void)
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;			   //基本结构体变量定义
		TIM_OCInitTypeDef  		TIM_OCInitStructure;               //输出结构体变量定义
		TIM_BDTRInitTypeDef  	TIM_BDTRInitStructure;			   //死区刹车结构体变量定义
		GPIO_InitTypeDef GPIO_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;  //TIM1输出
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;						   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
			 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;//TIM1输出
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;						   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		TIM_DeInit(TIM1);
		TIM_TimeBaseStructure.TIM_Prescaler = 0;					   //TIM基本初始化
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1 ;//中央对齐计数模式,输出比较标志位只有在比较器向上计算被设置
		TIM_TimeBaseStructure.TIM_Period = 7199;					   //PWM 16K
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;				   

		TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);

		TIM_OCInitStructure.TIM_OCMode =  TIM_OCMode_PWM1; 		   //TIM输出通道初始化
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
		TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;                  
		TIM_OCInitStructure.TIM_Pulse =400; 
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
		TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;         
		TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;//TIM_OCIdleState_Set;
		TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;          

		TIM_OC1Init(TIM1,&TIM_OCInitStructure); 
		TIM_OC2Init(TIM1,&TIM_OCInitStructure);
		TIM_OC3Init(TIM1,&TIM_OCInitStructure);

		TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;	//死区刹车初始化
		TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
		TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF; 
		TIM_BDTRInitStructure.TIM_DeadTime = 100;
		TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;	 //如打开，开机无输出且状态紊乱？？？？
		TIM_BDTRInitStructure.TIM_BreakPolarity =  TIM_BreakPolarity_Low ;
		TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;

		TIM_BDTRConfig(TIM1,&TIM_BDTRInitStructure);


		TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);  //使能捕获比较寄存器预装载（通道1）

		TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);	 //使能捕获比较寄存器预装载（通道2）

		TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable);	 //使能捕获比较寄存器预装载（通道3）


		
//		TIM_ClearITPendingBit( TIM1, TIM_IT_COM);
//		TIM_ITConfig(TIM1, TIM_IT_COM ,ENABLE);
		TIM_ClearFlag(TIM1,TIM_FLAG_Update);
		TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
		TIM_CtrlPWMOutputs(TIM1,ENABLE);

//		TIM_Cmd(TIM1,ENABLE);
		TIM_Cmd(TIM1,DISABLE);
		
		/* Enable the Update Interrupt */
		NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
}




void TIM3_IRQHandler(void)
{ 
   if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
   {
//		 TIM_Cmd(TIM3,DISABLE);
//		 TIM3->CNT=0;
//		 TIM_Cmd(TIM2,ENABLE);
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
			TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	 }
}

void TIM2_IRQHandler(void)
{ 
   if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
   {
//		 TIM_Cmd(TIM2,DISABLE);
//		 TIM2->CNT=0;
//		 TIM_Cmd(TIM1,ENABLE);
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
			TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	 }
}

void TIM1_UP_IRQHandler(void)
{
//	TIM_Cmd(TIM1,DISABLE);
//	TIM1->CNT=0;
//	TIM_Cmd(TIM3,ENABLE);
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);

}


