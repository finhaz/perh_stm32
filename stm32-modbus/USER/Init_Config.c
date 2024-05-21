#include "stm32f10x.h"
#include "Init_Config.h"




extern void GPIO_Configuration(void);			//GPIO
//extern void RCC_Configuration(void);			//RCC
//extern void ADC_Configuration(void);			//ADC
extern void USART1_Configuration(void);			//USART1
extern void NVIC_Configuration(void);			//NVIC
extern void Timer_Configuration(void);			//NVIC


void Init_Config(void)
{
 
// GPIO_Configuration();
 //SHT10_Config();	   
 
	NVIC_Configuration();
  
	Timer_Configuration();
 
	USART1_Configuration();

}
