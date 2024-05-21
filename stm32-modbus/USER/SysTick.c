#include "SysTick.h"
#include "stm32f10x.h"

static __IO u32 TimingDelay;

void SysTick_Init(void)
{
	if(SysTick_Config(SystemCoreClock/100000))
	{
		while(1);
	}
	
	//�رյδ�ʱ��
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void TimingDelay_Decrement(void)
{
	if(TimingDelay != 0x00)
	{
		TimingDelay--;
	}
}

void Delay_us(__IO u32 nTime)
{
	TimingDelay = nTime;
	
	//ʹ�ܵδ�ʱ��
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	
	while(TimingDelay != 0);
}
