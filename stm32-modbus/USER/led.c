
#include "led.h"

/*
		* ������      :   LED_GPIO_Config
		* ��������    :		����LED�õ��Ĺ��ܺ���		
		* ����        :		��
		* ���        :		��
*/

void LED_GPIO_Config(void)
{
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*����GPIOF������ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);
	
	/*ѡ��Ҫ���Ƶ�GPIOF����*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9| GPIO_Pin_13;
	
	/*��������ģʽΪͨ���������*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	
	/*������������Ϊ50MHz*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	/*���ÿ⺯������ʼ��GPIOF*/
	GPIO_Init(GPIOF,&GPIO_InitStructure);
	
	/*�ر�����led��*/
	GPIO_ResetBits(GPIOF,GPIO_Pin_13);//��GPIOF11λ�͵�ƽ��485����ʹ��
	GPIO_SetBits(GPIOF,GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 );
	
}
