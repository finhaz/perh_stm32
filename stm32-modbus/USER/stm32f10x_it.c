/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "Init_Config.h"
#include "uart1.h"
#include "modbus.h"

extern u8 Uart1_rev_flag;
extern u8 recenum;
extern u8 sendnum;
extern u8 ReceBuf[32];	        // ���ջ�����
extern u8 SendBuf[32];			//���ͻ�����
extern u32 crcData;
extern u8 usDataLen;
u8 sendcount = 0;
u8 head;
u8 num;
u8 num1;
u8 data_head_flag;
extern void TimingDelay_Decrement(void);
//extern u8 arr[256];
u8 flag = 1;


/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	TimingDelay_Decrement();
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/
void TIM2_IRQHandler(void)
{ 
   if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)//�ж��Ƿ���TIM2�����ж�
   {
		TIM_Cmd(TIM2,DISABLE); //�ص���ʱ��2 
		TIM_SetCounter(TIM2, 0);//�������ֵ0
		if(recenum >= 8)
		{
			Uart1_rev_flag = 1;//�������һ֡����λ��־λ��֪ͨ���������ý��մ�����
			usDataLen=recenum-2;
			USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//ʧ�ܴ���1�����ж�
		}
		recenum = 0;
	//	GPIO_SetBits(GPIOF,GPIO_Pin_13);//485����ʹ��
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//���TIM2���жϴ�����λ
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);//���TIM2�������־λ
	}
}



void USART1_IRQHandler(void)
{	
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)//�����ж�
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);//���USART1�жϴ�����λRXNE(RXNE=0)
		if(Uart1_rev_flag != 1)
		{
			if(recenum < 12)//���շ�8�ֽ����� ��11�ֽ�����
			{
				ReceBuf[recenum] = USART1->DR;
				recenum++;
				TIM_Cmd(TIM2, ENABLE);
				TIM_SetCounter(TIM2, 0);
				TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);	
			}
		}

	}		
	/*if(USART_GetITStatus(USART1,USART_IT_TXE)!=RESET)
	{
		USART_ClearITPendingBit(USART1,USART_IT_TXE);//���USART1�жϴ�����λRXNE(RXNE=0)
		USART_SendData(USART1,ReceBuf[sendnum]);
		sendnum++;
		if(sendnum == 8)
		{
			LED4(ON);
			sendnum = 0;
			USART_ITConfig(USART1,USART_IT_TXE,DISABLE);
			//USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//�򿪽����ж�ʹ��
			//GPIO_ResetBits(GPIOF,GPIO_Pin_13);//485ʹ�ܽ���
		}
	}*/
	//���-������������Ҫ�����SR�����λ,�ٶ�DR�Ĵ��� �������������жϵ�����
	if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)!=RESET)
	{
		USART_ClearFlag(USART1,USART_FLAG_ORE);	//�����λ
		USART_ReceiveData(USART1);				//��DR
	} 
}

