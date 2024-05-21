#include "sys.h"
#include <stdio.h>
#include <string.h> 
#include "delay.h"
#include "usart.h"
#include "24l01.h"
#include "count.h"




extern u8 setflag;



int main(void)
 {		

	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
	NRF24L01_Init();	 
	 
	while(NRF24L01_Check())	//���NRF24L01�Ƿ���λ	
	{
	} 
	NRF24L01_RT_Mode();
	TIM1_Int_Init(1000-1,72-1);//1ms

	
	while(1)
	{
		if(!setflag)
    {
		if(NRF24L01_IRQ==0)
			NRF_Get_State();
		}
		else
		{
			sendtorec();
		}

	 
	}
 }

 
 
