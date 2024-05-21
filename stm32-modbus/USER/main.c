/************************************************************************

	   * 文件名     ：main.c
	   * 描述       ：3.5.0版本建立的工程模板
	   * 实验平台   : stm32f103c8t6 core
	   * 功能       ：基于STM32的modbus-RTU-uart通信
	   * 作者       ：fin
	   * 日期       ：2017-4-18

*************************************************************************/

#include "stm32f10x.h"
#include "Init_Config.h"
#include "modbus.h"
#include "Systick.h"

u8 ReceBuf[32];	            //接收缓冲区
u8 SendBuf[32];			    //发送缓冲区
u8 recenum;
u8 sendnum;
u32 crcData;
u8  usDataLen;
u8 Uart1_rev_flag;

/*
		* 函数名      :   main.c
		* 功能描述    :		主函数		
		* 输入        :		无
		* 输出        :		无
*/
int main(void)
{

	
	RCC_Configuration();

	Init_Config();
		 
	SysTick_Init();
	
	while(1)
	{

		if(Uart1_rev_flag == 0x01)
		{	
			Uart1_rev_flag = 0x00;//接收一帧数据标志清零
	  
			ParseRecieve8();//数据帧处理函数

			USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//允许接收中断
		}

    }
}
