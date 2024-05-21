#include "stm32f10x.h"

void GPIO_Configuration(void)
{
         
		  GPIO_InitTypeDef GPIO_InitStructure;
          RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

  
          //PB0 SDA 推挽输出
          GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
          GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
          GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
          GPIO_Init(GPIOB, &GPIO_InitStructure);
          //PB1 SCK 推挽输出 sck不用输入
          GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
          GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
          GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
          GPIO_Init(GPIOB, &GPIO_InitStructure);
		  

}


