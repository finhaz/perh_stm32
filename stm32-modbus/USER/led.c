
#include "led.h"

/*
		* 函数名      :   LED_GPIO_Config
		* 功能描述    :		配置LED用到的功能函数		
		* 输入        :		无
		* 输出        :		无
*/

void LED_GPIO_Config(void)
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*开启GPIOF的外设时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);
	
	/*选择要控制的GPIOF引脚*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9| GPIO_Pin_13;
	
	/*设置引脚模式为通用推挽输出*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	
	/*设置引脚速率为50MHz*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	/*调用库函数，初始化GPIOF*/
	GPIO_Init(GPIOF,&GPIO_InitStructure);
	
	/*关闭所有led灯*/
	GPIO_ResetBits(GPIOF,GPIO_Pin_13);//是GPIOF11位低电平，485接收使能
	GPIO_SetBits(GPIOF,GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 );
	
}
