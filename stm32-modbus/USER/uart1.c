#include "uart1.h"

/*����1���ú���*/
void USART1_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//GPIO���ýṹ��
	USART_InitTypeDef USART_InitStructure;//�������ýṹ��
	
	/*ʹ��ʹ�õ�����ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	/*GPIOA_Pin_9----USART1_Tx���Ͷ�*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*GPIOA_Pin_10----USART1_Rx���ն�*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*GPIOA_Pin_0----�շ�ʹ�ܶˣ�0���գ�1����*/
	/*GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��485�����������
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);//ʹ���ʼ��Ϊ��������*/
	GPIO_ResetBits(GPIOF,GPIO_Pin_11);
	
	/*USART1ͨ������*/
	USART_InitStructure.USART_BaudRate = 115200;//������Ϊ115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8λ����λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1λֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ��������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//���������ģʽ

	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//ʹ�ܴ���1�����ж�
	USART_ITConfig(USART1,USART_IT_TXE,DISABLE);//�رմ���1�����ж�
	
	USART_Cmd(USART1, ENABLE);//ʹ�ܴ���1
}

/*����һ���ַ�*/
void usart_sent_char(USART_TypeDef* USARTx,u8 ch )
{  
	   USART_SendData( USARTx,ch);
	   while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	   USART_ClearFlag(USARTx, USART_FLAG_TXE);
} 

/*����һ���ַ���*/
void usart_sent_string(USART_TypeDef* USARTx,char *ch)
{
	while(*ch!='\0')
	{
		usart_sent_char( USARTx,*ch );
		ch++;
	}
}

/*��ֵ��ʾ������*/
/*void DataDisplay()
{
	u8 crcDataHi;
	u8 crcDataLo;
	crcData = crc16(ReceBuf,usDataLen);//�ӽ��յ���Ч��������CRCУ����
	crcDataLo = crcData/256; //	 132 84
	crcDataHi = crcData%256; //	 10  0A
	if((crcDataHi == ReceBuf[7])&&(crcDataLo == ReceBuf[6]))//�Ƚ�����CRC�뷢�͹�����CRC�Ƿ�һ��
	{
		LED4(ON);
		if(ReceBuf[0] == 0x01)//�豸��ַƥ��
		{
			SendBuf[0] = ReceBuf[0];
			SendBuf[1] = ReceBuf[1];
			ReadNum = ((ReceBuf[4] << 8) | ReceBuf[5]);//��ȡ�Ĵ�����
			SendCount = ReadNum*2 + 5;
			switch(ReadNum)//�����յ����ֽ������ز�ͬ������
			{
				case 1:
				{
					SendBuf[2] = ReadNum;//��ʾֵ���صĵ����ֽ���
					SendBuf[3] = num/256;
					SendBuf[4] = num%256;
					crcData = crc16(SendBuf,3+ReadNum*2);
					crcDataLo = crcData/256;//CRCУ���8λ
					crcDataHi = crcData%256;//CRCУ���8λ
					SendBuf[SendCount-2] = crcDataLo;//CRCУ���λ���ط����ݵĵ����ڶ�λ
					SendBuf[SendCount-1] = crcDataHi;//CRCУ���λ���ط����ݵĵ�����һλ
					for(i = 0; i < SendCount; i++)
					{
						usart_sent_char(USART2,SendBuf[i]);
					}
					GPIO_ResetBits(GPIOA,GPIO_Pin_0);//��485����ʹ�ܣ���ֹ����ʹ��
					USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//�򿪴���2�����ж�ʹ��
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

/*�жϴ�����*/
/*void USART2_IRQHandler(void)
{
	LED2(ON);
	if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET)
	{
		//�����յ���һ�ֽ����ݷ�����ջ�����
		LED1(ON);
		ReceBuf[ReceCount++] = USART_ReceiveData(USART2);
		usDataLen = ReceCount - 2;	
		if(ReceCount >= 8)
		{
			LED2(ON);
			switch(ReceBuf[1])//�жϽ��ջ������ڶ�λ�Ĺ��ܺ�
			{
				case 0x03:
					if(ReceCount == 8)
					{
						ReceCount = 0;
						USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);//�رմ���2�����ж�ʹ��
						GPIO_SetBits(GPIOA,GPIO_Pin_0);//�ر�485����ʹ�ܣ��򿪷���ʹ��
						LED3(ON);
						DataDisplay();//�ڴ���������ʾ��ֵ
						break;
					}		
				case 0x10:
					if(ReceCount == 11)
					{
						ReceCount = 0;
						USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);//�رմ���2�����ж�ʹ��
						GPIO_SetBits(GPIOA,GPIO_Pin_0);//�ر�485����ʹ�ܣ��򿪷���ʹ��
						DataModify();//�޸ĳ������
						break;
					}				
			}
}*/

