#include "uart1.h"

/*串口1配置函数*/
void USART1_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//GPIO配置结构体
	USART_InitTypeDef USART_InitStructure;//串口配置结构体
	
	/*使能使用的外设时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	/*GPIOA_Pin_9----USART1_Tx发送端*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*GPIOA_Pin_10----USART1_Rx接收端*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*GPIOA_Pin_0----收发使能端，0接收，1发送*/
	/*GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化485方向控制引脚
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);//使其初始化为接收允许*/
	GPIO_ResetBits(GPIOF,GPIO_Pin_11);
	
	/*USART1通信配置*/
	USART_InitStructure.USART_BaudRate = 115200;//波特率为115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8位数据位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1位停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//发送与接收模式

	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//使能串口1接收中断
	USART_ITConfig(USART1,USART_IT_TXE,DISABLE);//关闭串口1发送中断
	
	USART_Cmd(USART1, ENABLE);//使能串口1
}

/*发送一个字符*/
void usart_sent_char(USART_TypeDef* USARTx,u8 ch )
{  
	   USART_SendData( USARTx,ch);
	   while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	   USART_ClearFlag(USARTx, USART_FLAG_TXE);
} 

/*发送一个字符串*/
void usart_sent_string(USART_TypeDef* USARTx,char *ch)
{
	while(*ch!='\0')
	{
		usart_sent_char( USARTx,*ch );
		ch++;
	}
}

/*数值显示处理函数*/
/*void DataDisplay()
{
	u8 crcDataHi;
	u8 crcDataLo;
	crcData = crc16(ReceBuf,usDataLen);//从接收的有效数据生成CRC校验码
	crcDataLo = crcData/256; //	 132 84
	crcDataHi = crcData%256; //	 10  0A
	if((crcDataHi == ReceBuf[7])&&(crcDataLo == ReceBuf[6]))//比较生成CRC与发送过来的CRC是否一致
	{
		LED4(ON);
		if(ReceBuf[0] == 0x01)//设备地址匹配
		{
			SendBuf[0] = ReceBuf[0];
			SendBuf[1] = ReceBuf[1];
			ReadNum = ((ReceBuf[4] << 8) | ReceBuf[5]);//读取寄存器数
			SendCount = ReadNum*2 + 5;
			switch(ReadNum)//根据收到的字节数返回不同的数据
			{
				case 1:
				{
					SendBuf[2] = ReadNum;//显示值返回的的总字节数
					SendBuf[3] = num/256;
					SendBuf[4] = num%256;
					crcData = crc16(SendBuf,3+ReadNum*2);
					crcDataLo = crcData/256;//CRC校验高8位
					crcDataHi = crcData%256;//CRC校验低8位
					SendBuf[SendCount-2] = crcDataLo;//CRC校验低位给回发数据的倒数第二位
					SendBuf[SendCount-1] = crcDataHi;//CRC校验高位给回发数据的倒数第一位
					for(i = 0; i < SendCount; i++)
					{
						usart_sent_char(USART2,SendBuf[i]);
					}
					GPIO_ResetBits(GPIOA,GPIO_Pin_0);//打开485接收使能，禁止发送使能
					USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//打开串口2接收中断使能
					break;
				}
				case 2:
				{
					break;
				}
					
			}
		}
	}
}
*/

/*中断处理函数*/
/*void USART2_IRQHandler(void)
{
	LED2(ON);
	if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET)
	{
		//将接收到的一字节数据放入接收缓冲区
		LED1(ON);
		ReceBuf[ReceCount++] = USART_ReceiveData(USART2);
		usDataLen = ReceCount - 2;	
		if(ReceCount >= 8)
		{
			LED2(ON);
			switch(ReceBuf[1])//判断接收缓冲区第二位的功能号
			{
				case 0x03:
					if(ReceCount == 8)
					{
						ReceCount = 0;
						USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);//关闭串口2接收中断使能
						GPIO_SetBits(GPIOA,GPIO_Pin_0);//关闭485接收使能，打开发送使能
						LED3(ON);
						DataDisplay();//在触摸屏上显示数值
						break;
					}		
				case 0x10:
					if(ReceCount == 11)
					{
						ReceCount = 0;
						USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);//关闭串口2接收中断使能
						GPIO_SetBits(GPIOA,GPIO_Pin_0);//关闭485接收使能，打开发送使能
						DataModify();//修改程序参数
						break;
					}				
			}
}*/

