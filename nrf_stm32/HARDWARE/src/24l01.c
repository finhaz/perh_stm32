#include <string.h>
#include "24l01.h"
#include "delay.h"
#include "spi.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//NRF24L01��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/13
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
    
const u8 TX_ADDRESS[TX_ADR_WIDTH]={0xFF,0xFF,0xFF,0xFF,0xFF}; //���͵�ַ
const u8 RX_ADDRESS[RX_ADR_WIDTH]={0xFF,0xFF,0xFF,0xFF,0xFF}; //���͵�ַ

u8 rec_buf[33]; //��һλ����Ϊ��������nrf����շ�32�ֽڣ��ַ���Ĭ����0��Ϊ������
u8 tra_buf[32];

void nrf_delay_us(u16 us)
{

	u16 t;
	while(us--)
	  for(t=10;t>0;t--);

}



//��ʼ��24L01��IO��
void NRF24L01_Init(void)
{ 	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);	 //ʹ��PA B�˿�ʱ��
    		 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				 //PB12���� ��ֹW25X�ĸ��� CE
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	//��ʼ��ָ��IO
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	//PA8 ���� 	  CSN
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��ָ��IO
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;    //IRQ
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN_FLOATING ; //PB5 ��������  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	

  SPI2_Init();    		//��ʼ��SPI	 
 

			 
	NRF24L01_CE=0; 			//ʹ��24L01
	NRF24L01_CSN=1;			//SPIƬѡȡ��  
	 		 	 
}
//���24L01�Ƿ����
//����ֵ:0���ɹ�;1��ʧ��	
u8 NRF24L01_Check(void)
{
	u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	u8 i;
	SPI2_SetSpeed(SPI_BaudRatePrescaler_4); //spi�ٶ�Ϊ9Mhz��24L01�����SPIʱ��Ϊ10Mhz��   	 
	NRF24L01_Write_Buf(WRITE_REG_NRF+TX_ADDR,buf,5);//д��5���ֽڵĵ�ַ.	
	NRF24L01_Read_Buf(TX_ADDR,buf,5); //����д��ĵ�ַ  
	for(i=0;i<5;i++)if(buf[i]!=0XA5)break;	 							   
	if(i!=5)return 1;//���24L01����	
	return 0;		 //��⵽24L01
}	 	 
//SPIд�Ĵ���
//reg:ָ���Ĵ�����ַ
//value:д���ֵ
u8 NRF24L01_Write_Reg(u8 reg,u8 value)
{
	u8 status;	
   	NRF24L01_CSN=0;                 //ʹ��SPI����
  	status =SPI2_ReadWriteByte(reg);//���ͼĴ����� 
  	SPI2_ReadWriteByte(value);      //д��Ĵ�����ֵ
  	NRF24L01_CSN=1;                 //��ֹSPI����	   
  	return(status);       			//����״ֵ̬
}
//��ȡSPI�Ĵ���ֵ
//reg:Ҫ���ļĴ���
u8 NRF24L01_Read_Reg(u8 reg)
{
	u8 reg_val;	    
 	NRF24L01_CSN = 0;          //ʹ��SPI����		
  	SPI2_ReadWriteByte(reg);   //���ͼĴ�����
  	reg_val=SPI2_ReadWriteByte(0XFF);//��ȡ�Ĵ�������
  	NRF24L01_CSN = 1;          //��ֹSPI����		    
  	return(reg_val);           //����״ֵ̬
}	
//��ָ��λ�ö���ָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ 
u8 NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len)
{
	u8 status,u8_ctr;	       
  	NRF24L01_CSN = 0;           //ʹ��SPI����
  	status=SPI2_ReadWriteByte(reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬   	   
 	for(u8_ctr=0;u8_ctr<len;u8_ctr++)pBuf[u8_ctr]=SPI2_ReadWriteByte(0XFF);//��������
  	NRF24L01_CSN=1;       //�ر�SPI����
  	return status;        //���ض�����״ֵ̬
}
//��ָ��λ��дָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
	u8 status,u8_ctr;	    
 	NRF24L01_CSN = 0;          //ʹ��SPI����
  	status = SPI2_ReadWriteByte(reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
  	for(u8_ctr=0; u8_ctr<len; u8_ctr++)SPI2_ReadWriteByte(*pBuf++); //д������	 
  	NRF24L01_CSN = 1;       //�ر�SPI����
  	return status;          //���ض�����״ֵ̬
}				   
//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:�������״��
u8 NRF24L01_TxPacket(u8 *txbuf)
{
	u8 sta;
 	SPI2_SetSpeed(SPI_BaudRatePrescaler_8);//spi�ٶ�Ϊ9Mhz��24L01�����SPIʱ��Ϊ10Mhz��   
	NRF24L01_CE=0;
  	NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//д���ݵ�TX BUF  32���ֽ�
 	NRF24L01_CE=1;//��������	   
	while(NRF24L01_IRQ!=0);//�ȴ��������
	sta=NRF24L01_Read_Reg(STATUS);  //��ȡ״̬�Ĵ�����ֵ	   
	NRF24L01_Write_Reg(WRITE_REG_NRF+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	if(sta&MAX_TX)//�ﵽ����ط�����
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);//���TX FIFO�Ĵ��� 
		return MAX_TX; 
	}
	if(sta&TX_OK)//�������
	{
		return TX_OK;
	}
	return 0xff;//����ԭ����ʧ��
}
//����NRF24L01����һ������
//rxbuf:�����������׵�ַ
//����ֵ:0��������ɣ�1��û�յ�����
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 sta;		    							   
	SPI2_SetSpeed(SPI_BaudRatePrescaler_8); //spi�ٶ�Ϊ9Mhz��24L01�����SPIʱ��Ϊ10Mhz��   
	sta=NRF24L01_Read_Reg(STATUS);  //��ȡ״̬�Ĵ�����ֵ    	 
	NRF24L01_Write_Reg(WRITE_REG_NRF+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	if(sta&RX_OK)//���յ�����
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//��ȡ����
		NRF24L01_Write_Reg(FLUSH_RX,0xff);//���RX FIFO�Ĵ��� 
		return 0; 
	}	   
	return 1;//û�յ��κ�����
}					    
//�ú�����ʼ��NRF24L01��RXģʽ
//����RX��ַ,дRX���ݿ��,ѡ��RFƵ��,�����ʺ�LNA HCURR
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
void NRF24L01_RX_Mode(void)
{
		NRF24L01_CE=0;	  
  	NRF24L01_Write_Buf(WRITE_REG_NRF+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);//дRX�ڵ��ַ
	  
  	NRF24L01_Write_Reg(WRITE_REG_NRF+EN_AA,0x01);    //ʹ��ͨ��0���Զ�Ӧ��    
//  	NRF24L01_Write_Reg(WRITE_REG_NRF+EN_AA,0x00);    //�ر�ͨ��0���Զ�Ӧ��    	
  	NRF24L01_Write_Reg(WRITE_REG_NRF+EN_RXADDR,0x01);//ʹ��ͨ��0�Ľ��յ�ַ  	 
  	NRF24L01_Write_Reg(WRITE_REG_NRF+RF_CH,0);	     //����RFͨ��Ƶ��	,2400+ RH_CH mhz	  
  	NRF24L01_Write_Reg(WRITE_REG_NRF+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ�� 	    
  	NRF24L01_Write_Reg(WRITE_REG_NRF+RF_SETUP,0x0f);//����TX�������,0db����,2Mbps,���������濪��   
  	NRF24L01_Write_Reg(WRITE_REG_NRF+CONFIG, 0x0f);//���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 

  	NRF24L01_CE = 1; //CEΪ��,�������ģʽ 
}						 
//�ú�����ʼ��NRF24L01��TXģʽ
//����TX��ַ,дTX���ݿ��,����RX�Զ�Ӧ��ĵ�ַ,���TX��������,ѡ��RFƵ��,�����ʺ�LNA HCURR
//PWR_UP,CRCʹ��
//��CE��ߺ�,������TXģʽ,�����Խ���������		   
//CEΪ�ߴ���10us,����������.	 
void NRF24L01_TX_Mode(void)
{														 
		NRF24L01_CE=0;	    
  	NRF24L01_Write_Buf(WRITE_REG_NRF+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);//дTX�ڵ��ַ 
  	NRF24L01_Write_Buf(WRITE_REG_NRF+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	  

  	NRF24L01_Write_Reg(WRITE_REG_NRF+EN_AA,0x01);     //ʹ��ͨ��0���Զ�Ӧ��    
  	NRF24L01_Write_Reg(WRITE_REG_NRF+EN_RXADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ  
  	NRF24L01_Write_Reg(WRITE_REG_NRF+SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
  	NRF24L01_Write_Reg(WRITE_REG_NRF+RF_CH,0);       //����RFͨ��Ϊ0,ͨѶƵ��2400+ RH_CH mhz
  	NRF24L01_Write_Reg(WRITE_REG_NRF+RF_SETUP,0x0f);  //����TX�������,0db����,2Mbps,���������濪��   
  	NRF24L01_Write_Reg(WRITE_REG_NRF+CONFIG,0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
		NRF24L01_CE=1;//CEΪ��,10us����������
}		  

/****NRF24L01�շ�ģʽ��ʼ��*********/
void NRF24L01_RT_Mode(void)
{

	clear_status();
	NRF24L01_CE=0;
	
  NRF24L01_Write_Buf(WRITE_REG_NRF+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);//дTX�ڵ��ַ 	
	NRF24L01_Write_Buf(WRITE_REG_NRF+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);//дRX�ڵ��ַ
	NRF24L01_Write_Reg(WRITE_REG_NRF+EN_AA,0x01);     //ʹ��ͨ��0���Զ�Ӧ��    
  NRF24L01_Write_Reg(WRITE_REG_NRF+EN_RXADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ  
	NRF24L01_Write_Reg(WRITE_REG_NRF+RF_CH,0);       //����RFͨ��Ϊ0,ͨѶƵ��2400+ RH_CH mhz ˫��һ��
	NRF24L01_Write_Reg(WRITE_REG_NRF+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ��
	NRF24L01_Write_Reg(WRITE_REG_NRF+RF_SETUP,0x0f);  //����TX�������,0db����,2Mbps,���������濪��   
  NRF24L01_Write_Reg(WRITE_REG_NRF+CONFIG,0x0f);    //���û�������ģʽ�Ĳ���:����ģʽ��1~16λCRCУ�飬IRQ��ʾ�����ж�
	
	NRF24L01_CE=1;
	

}

/******nrf24l01�շ�����*************/

u8 sta; //״̬�Ĵ����洢


void nrf_RxTx(u8 mod_nrf ,u8 *buff)
{
	static u8 mod_nrf_b; //static ��ַ���ͷ�
	
	/*****���뷢��ģʽ****/
	
	//******���뷢��ģʽ******
	if(mod_nrf == 't')
	{
		if(mod_nrf_b != 't')
		{	
			mod_nrf_b = 't';
			NRF24L01_CE = 0; 
			NRF24L01_Write_Reg(WRITE_REG_NRF+STATUS,NOP); 	//����жϱ�־
			NRF24L01_Write_Reg(FLUSH_TX,NOP);			//���TX_FIFO�Ĵ��� 
			NRF24L01_Write_Reg(WRITE_REG_NRF + CONFIG,0x7e);//IRQ���Ų���ʾ�ж� �ϵ� ����ģʽ  1~16CRCУ��
			NRF24L01_CE = 1;
			//nrf_delay_us(130);//��CE = 0 �� CE = 1��������ģʽ���շ�ģʽ����Ҫ���130us		   
		}
		
//******��������******
		NRF24L01_CE= 0;			//StandBy Iģʽ	
		NRF24L01_Write_Buf(WRITE_REG_NRF + RX_ADDR_P0, (u8*)RX_ADDRESS, RX_ADR_WIDTH); // װ�ؽ��ն˵�ַ
		NRF24L01_Write_Buf(WR_TX_PLOAD,buff,TX_PLOAD_WIDTH); 			 // װ������
		NRF24L01_CE= 1;		 //�ø�CE�������ݷ���
		
		nrf_delay_us(130);//��CE = 0 �� CE = 1��������ģʽ���շ�ģʽ����Ҫ���130us	
		nrf_delay_us(100); //����������һ��ʱ��	 �ȷ����ٶȽϿ� ��ʱ���ԱȽ�����
		nrf_delay_us(10);
		sta = NRF24L01_Read_Reg(STATUS);//��ȡ״̬�Ĵ�����ֵ
		NRF24L01_Write_Reg(WRITE_REG_NRF+STATUS,sta);//�����Ӧ���ж�
					
		if(sta&TX_OK)//���ͳɹ������tx fifo�Ĵ���  TX_DSλΪ1
		{	
			NRF24L01_CE= 0;			
			NRF24L01_Write_Reg(FLUSH_TX,NOP); //���tx fifo�Ĵ���	//********��Ҫ*********
			NRF24L01_CE= 1;
		}								
				
	} 
//******�������ģʽ******
	else if(mod_nrf == 'r')//����ģʽ
	{
		if(mod_nrf_b != 'r')
		{
			mod_nrf_b = 'r';
			NRF24L01_CE = 0; 
			NRF24L01_Write_Reg(WRITE_REG_NRF+STATUS,NOP);	//����жϱ�־
			NRF24L01_Write_Reg(FLUSH_RX,NOP); 			//���RX_FIFO�Ĵ���
			NRF24L01_Write_Reg(WRITE_REG_NRF+ CONFIG, 0x0f);//IRQ������ʾ RX_RD�ж� �ϵ� ����ģʽ   1~16CRCУ��   
			NRF24L01_CE = 1; 
			nrf_delay_us(130);//��CE = 0 �� CE = 1��������ģʽ���շ�ģʽ����Ҫ���130us
		}		
		nrf_delay_us(500); //������ ֵ�ɵ�  ����������һ��ʱ��
		sta = NRF24L01_Read_Reg(STATUS);
		NRF24L01_Write_Reg(WRITE_REG_NRF+STATUS,sta);    
		if(sta&RX_OK)      //RX_DR λΪ �յ�����
		{
			NRF24L01_CE = 0;  
			NRF24L01_Read_Buf(RD_RX_PLOAD,buff,RX_PLOAD_WIDTH);//��ȡ���� ��������
			NRF24L01_Write_Reg(FLUSH_RX,NOP);//���rx fifo�Ĵ���	���ݲ����� 
			NRF24L01_CE = 1;
		} 	 	
	 }

	
	
	
}






void clear_status(void)
{
	
	u8 status;
  status=NRF24L01_Read_Reg(STATUS);
	NRF24L01_Write_Reg(WRITE_REG_NRF+STATUS, status);
	NRF24L01_Write_Reg(FLUSH_RX, NOP);    // ���RX FIFO�Ĵ��� 
	NRF24L01_Write_Reg(FLUSH_TX,NOP);      /* ���TX FIFO�Ĵ��� */
	
}



u8 NRF_Get_State(void)
{
	  u8 status;
		u8 rf_rec_flag;	
	  status=NRF24L01_Read_Reg(STATUS);
		NRF24L01_Write_Reg(WRITE_REG_NRF+STATUS, status); /*���TX_DS��MAX_RT��RX_DR�жϱ�־*/ 
	  if(status & RX_OK)
		{
				NRF24L01_Read_Buf(RD_RX_PLOAD,rec_buf,RX_PLOAD_WIDTH);//��ȡ����
			  NRF24L01_Write_Reg(FLUSH_RX, NOP);    // ���RX FIFO�Ĵ��� 
				printf("%s",rec_buf);
				rf_rec_flag = RX_OK; 
		}
		else if(status & MAX_TX) /* �ﵽ����ط����� */
		{	
			NRF24L01_Write_Reg(FLUSH_TX,NOP);      /* ���TX FIFO�Ĵ��� */
			rf_rec_flag = MAX_TX;
		}
		else if(status & TX_OK)/* ������� */
		{
			NRF24L01_Write_Reg(FLUSH_TX,NOP);      /* ���TX FIFO�Ĵ��� */
			rf_rec_flag = TX_OK;	
		}
		else 
			rf_rec_flag = 0;   /* û�յ��κ����� */
		
		return rf_rec_flag;
}


extern u8 setflag;
extern u8 cnt;

void sendtorec(void)
{
		
		
		while((sta&TX_OK)!=TX_OK)
		{
			nrf_RxTx('t',tra_buf);
		}
    memset(tra_buf,0,32);

    cnt=0;
    setflag=0;
    nrf_RxTx('r',rec_buf);     

    
}
