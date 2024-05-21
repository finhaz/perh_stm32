/************************************************************************

	   * �ļ���     ��main.c
	   * ����       ��3.5.0�汾�����Ĺ���ģ��
	   * ʵ��ƽ̨   : stm32f103c8t6 core
	   * ����       ������STM32��modbus-RTU-uartͨ��
	   * ����       ��fin
	   * ����       ��2017-4-18

*************************************************************************/

#include "stm32f10x.h"
#include "Init_Config.h"
#include "modbus.h"
#include "Systick.h"

u8 ReceBuf[32];	            //���ջ�����
u8 SendBuf[32];			    //���ͻ�����
u8 recenum;
u8 sendnum;
u32 crcData;
u8  usDataLen;
u8 Uart1_rev_flag;

/*
		* ������      :   main.c
		* ��������    :		������		
		* ����        :		��
		* ���        :		��
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
			Uart1_rev_flag = 0x00;//����һ֡���ݱ�־����
	  
			ParseRecieve8();//����֡������

			USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�
		}

    }
}
