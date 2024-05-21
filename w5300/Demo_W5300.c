#include<c8051F340.h>

#include"Device.h"
#include"Parameters.h"

#include"W5300.h"

/*********************** Delay Subroutine ***********************/
void Delay(unsigned int d)
{
	unsigned int i,j;

	i=timer0_cnt;
	do
	{
		j=timer0_cnt;
		if(j>=i)
			j=j-i;
		else
			j=65535-i+j;
	}while(j<d);
}

/***************************************************************************************
                                    C8051F340 initialization
It initializes IO port, Timer, UART, A/D system and I2C port
***************************************************************************************/
void C8051F340_Initialization(void)
{
	PCA0MD&=~0x40; 	/* Disable WDT */                   
	/*************** System clock *************/
	OSCICN=0x83;	/* SYSCLK derived from Internal H-F Oscillator divided by 1 */
	while((OSCICN & 0x40)==0);	/* Wait for High Frequency Clock ready */

	CLKMUL=0x80;	/* Clock Multiplier enabled */
	CLKMUL=0xc0;	/* Clock Multiplier enabled and Initialize it */
	while((CLKMUL & 0x20)==0);	/* Wait for CLOCK Multiplier Ready */

	CLKSEL=0x03;	/* Select 48MHz for SYSTEM CLOCK */

	/*************** IO port ***************/
	XBR0=0x01;		/* Enable UART0 and SMBUS Routed to Port */
	XBR1=0xc0;		/* Enable Crossbar */
	XBR2=0x00;
	
	/* Port 0 */
	P0MDOUT=0xd7;
	
	/* Port 1*/
	P1MDIN=0xfb;	/* P1.2 sets as Analog input*/
	P1MDOUT=0xf0;	/* P1.0, P1.1, P1.2, P1.3 sets as Open drain */
	P1SKIP=0x04;	/* P1.2 should be skipped by Crossbar */

	/* Port 2 */
	P2MDOUT=0xff;
	P2SKIP=0xff;

	/* Port 3 */
	P3MDOUT=0xff;
	P3SKIP=0xff;

	/* Port 4*/
	P4MDOUT=0xff;

	/* External mem access */
	EMI0CF=0x14;	/* Non-multiplexed mode, Split mode without bank select */
	EMI0TC=0x49;	/* Address setup time 1 CLK, WR and RD width 3 CLK, Address hold time 1 CLK*/

	/*************** Timer ***************/
	TCON=0x01;		/* INT0 is edge triggered */
	TMOD=0x21;		/* Timer1 is 8-bit timer with auto reload, timer0 is 16-bit timet */
	CKCON=0x02;		/* Timer0 and Timer1 use CLOCK defined by Prescaler, 1/4 system CLOCK */
	
	TH0=0xfc;
	TL0=0x18;
	ET0=1;			/* Enable Tomer0 interrupt */

	/*************** UART ***************/
	SCON0=0x10;		/* UART0 Receive enable*/
	TH1=204;		/* Timer1 as BAUD RATE Generator, 9600bps */
	TL1=204;
	ES0=1;			/* Enable UART0 interrupt */

	IT01CF=0x03;	/* Set P0.3 as INT0 */
	EX0=1;			/* Enable external 0 interrupt */
	EA=1;			/* Enable System interrupt */
	TR0=1;			/* Start Timer0 */
	TR1=1;			/* Start Timer1 */
}

/***************************************************************************************
                                   Load Parameters from Flash
***************************************************************************************/
void Load_Parameters(void)
{
	unsigned char i;
	unsigned char code *p_ptr;

	Para *param=(Para *)Parameters;

	if(D_INPUT1==0)
	{		/* Set Default parameters */
		param -> GATEWAY[0]= 192;				/* default Gateway */
		param -> GATEWAY[1]= 168;
		param -> GATEWAY[2]= 0;
		param -> GATEWAY[3]= 1;

		param -> SUBNET[0]= 255;				/* default subnet mask */
		param -> SUBNET[1]= 255;
		param -> SUBNET[2]= 255;
		param -> SUBNET[3]= 0;

		param -> IP[0]= 192;					/* default local IP address */
		param -> IP[1]= 168;
		param -> IP[2]= 0;
		param -> IP[3]= 20;
	
		for(i=0;i<8;i++)
		{
			param -> Sn_SPORT[i][0]= 0xc3;		/* default SOCKET port number*/
			param -> Sn_SPORT[i][1]= 0x50+i;
		}
		
		param->Sn_DIP[0][0]=192;
		param->Sn_DIP[0][1]=168;
		param->Sn_DIP[0][2]=0;
		param->Sn_DIP[0][3]=30;

		param->Sn_DPORT[0][0]=0xc7;
		param->Sn_DPORT[0][1]=0x38;

		for(i=0;i<8;i++)
			param -> Sn_MODE[i]= TCP_SERVER;	/* SOCKET operate in TCP server (default) */

		Erase_Page();
		Write_FALSH_Data();
	}

	param -> MAC_ADDR[0]= 0;				/* default MAC address */
	param -> MAC_ADDR[1]= 0xa0;
	param -> MAC_ADDR[2]= 0xc9;
	param -> MAC_ADDR[3]= 0x78;
	param -> MAC_ADDR[4]= 0x3e;
	param -> MAC_ADDR[5]= 0x10;

	p_ptr=(unsigned char code*)FLASH_MEM_BASE;
	for(i=0;i<4;i++)							/* Load Gateway IP */
	{
		param -> GATEWAY[i]=*p_ptr;
		p_ptr++;
	}

	for(i=0;i<4;i++)							/* Load Subnet MASK */
	{
		param -> SUBNET[i]=*p_ptr;
		p_ptr++;
	}
	
	p_ptr+=6;

	for(i=0;i<4;i++)							/* Load IP Address */
	{
		param -> IP[i]=*p_ptr;
		p_ptr++;
	}
	
	for(i=0;i<8;i++)							/* Load Socket Port Number */
	{
		param -> Sn_SPORT[i][0]=*p_ptr;
		p_ptr++;
		param -> Sn_SPORT[i][1]=*p_ptr;
		p_ptr++;
	}

	for(i=0;i<8;i++)							/* Load Socket Destination IP Address */
	{
		param -> Sn_DIP[i][0]=*p_ptr;
		p_ptr++;
		param -> Sn_DIP[i][1]=*p_ptr;
		p_ptr++;
		param -> Sn_DIP[i][2]=*p_ptr;
		p_ptr++;
		param -> Sn_DIP[i][3]=*p_ptr;
		p_ptr++;
	}

	for(i=0;i<8;i++)							/* Load Socket Destination Port Number */
	{
		param -> Sn_DPORT[i][0]=*p_ptr;	
		p_ptr++;
		param -> Sn_DPORT[i][1]=*p_ptr;
		p_ptr++;
	}

	for(i=0;i<8;i++)
	{
		param->Sn_MODE[i]=*p_ptr;

		if(param->Sn_MODE[i]>UDP_MODE)
			param->Sn_MODE[i]=TCP_SERVER;

		p_ptr++;
	}

	UART_RX=0;
}

/***************************************************************************************
                                    Flash Operation
***************************************************************************************/
/**************** Erase Flash memory ***************/
void Erase_Page(void)
{
	unsigned char xdata *ptr;

	EA=0;			/* Disable interrupt */

	FLKEY=0xa5;		/* First key code */
	FLKEY=0xf1;		/* Second key code */

	PSCTL|=0x02;	/* Set PSEE */
	PSCTL|=0x01;	/* Set PSWE */

	ptr=(unsigned char xdata*)FLASH_MEM_BASE;
	*ptr=0;

	PSCTL&=~0x01;	/* Clear PSWE */
	PSCTL&=~0x02;	/* Clear PSEE */

	EA=1;			/* Enable interrupt */
}

/*************** Write data to Flash **************/
void Write_FALSH_Data(void)
{
	unsigned char xdata *ptr;
	unsigned char i;

	EA=0;			/* Disable interrupt */
	
	PFE0CN&=~0x01;	/* Clear PFBWE */
	PSCTL|=0x01;	/* Set PSWE */
	PSCTL&=~0x02;	/* Clear PSEE */

	/* Write Data to Flash */
	for(i=0;i<90;i++)
	{
		FLKEY=0xa5;		/* First key code */
		FLKEY=0xf1;		/* Second key code */
		ptr=(unsigned char xdata*)(FLASH_MEM_BASE+i);
		*ptr=Parameters[i];
	}
	PSCTL&=~0x01;	/* Clear PSWE */

	EA=1;			/* Enable interrupt */	
}



/**************************************************************************************
Timer0 overflow interrupt handler
**************************************************************************************/
void Timer0_isr(void) interrupt 1 using 2
{
	TH0=0xfc;
	TL0=0x18;

	timer0_cnt++;
}

/**************************************************************************************
External 0 interrupt handler
**************************************************************************************/
void Ext0_isr(void) interrupt 0
{
	W5300_Interrupt_Process();
}

/**************************************************************************************
UART0 RX/TX interrupt handler
**************************************************************************************/
void USRT0_isr(void) interrupt 4 using 1
{
	if(RI0)				/* If receive 1 byte from UART0 */
	{
		RI0=0;
		if(UART_RX==0)
		{
			UART_RX_Data[UART_RX_idx]=SBUF0;
			if((UART_RX_Data[UART_RX_idx]>='a')&&(UART_RX_Data[UART_RX_idx]<='z'))
				UART_RX_Data[UART_RX_idx]-=('a'-'A');		/* Change to Captal */

			UART_CMD=1;
			SBUF0=UART_RX_Data[UART_RX_idx];

			UART_RX_idx++;
			UART_RX=1;
		}
	}

	if(TI0)				/* If transmit 1 byte in UART0 */
	{
		TI0=0;

		if(UART_CMD==0)
		{
			if(UART_TX_Data[UART_TX_idx]!=0)
			{
				SBUF0=UART_TX_Data[UART_TX_idx];
				UART_TX_idx++;
			}
			else
				UART_TXOK=1;
		}
	}
}

/**************************************************************************************
Process UART Data
**************************************************************************************/
//-----------------------------------------------------------------------------
//In setting mode, parse UART data
//-----------------------------------------------------------------------------
void tx_delimiter(unsigned char i)
{
	UART_TX_Data[i++]=CR;
	UART_TX_Data[i++]=LF;
	UART_TX_Data[i]=0;
}

void tx_OK(void)
{
	UART_TX_Data[0]='O';
	UART_TX_Data[1]='K';
	tx_delimiter(2);
}

void tx_data(void)
{
	unsigned char j,k,l;

	l=0;
	for(j=0;j<4;j++)
	{
		Hex_TO_Dec(temporary[j]);

		UART_TX_Data[l++]=(temp[1]&0x0f)+0x30;

		k=temp[0];
		k>>=4;
		UART_TX_Data[l++]=k+0x30;

		k=temp[0]&0x0f;
		UART_TX_Data[l++]=k+0x30;

		UART_TX_Data[l++]='.';
	}
	l--;
	tx_delimiter(l);
}

void tx_data1(unsigned int i)
{
	Hex_TO_Dec(i);
	UART_TX_Data[0]=(temp[2]&0x0f)+0x30;
	UART_TX_Data[1]=(temp[1]>>4)+0x30;
	UART_TX_Data[2]=(temp[1]&0x0f)+0x30;
	UART_TX_Data[3]=(temp[0]>>4)+0x30;
	UART_TX_Data[4]=(temp[0]&0x0f)+0x30;
	tx_delimiter(5);
}

unsigned char parse_data(void)			//Parse IP address, subnet mask data
{
	unsigned int i;
	unsigned char j,k;

	j=4;
	k=0;
	do
	{
		i=0;
		j++;
		while((UART_RX_Data[j]!='.')&&(UART_RX_Data[j]!=CR))
		{
			i<<=4;
			i=i|(UART_RX_Data[j]-0x30);
			j++;
		}
		if(i>0x255)
			return false;
		temporary[k]=Dec_TO_Hex(0,i);
		k++;
	}while(UART_RX_Data[j]!=CR);

	return true;
}

unsigned char parse_data1(void)			//Parse socket port number
{
	unsigned char i,k;
	unsigned int j;

	i=0;
	j=0;
	while(UART_RX_Data[5+i]!=CR)
		i++;
	if((i>5)||(i==0))
		return false;

	if(i==5)
	{
		k=UART_RX_Data[5]-0x30;
		j=UART_RX_Data[6]-0x30;
		j<<=4;
		j=j+(UART_RX_Data[7]-0x30);
		j<<=4;
		j=j+(UART_RX_Data[8]-0x30);
		j<<=4;
		j=j+(UART_RX_Data[9]-0x30);
	}
	else
	{
		for(k=0;k<i;k++)
		{
			j<<=4;
			j=j+(UART_RX_Data[5+k]-0x30);
		}
		k=0;
	}
	j=Dec_TO_Hex(k,j);
	temporary[0]=j>>8;
	temporary[1]=j;
	return true;
}

unsigned char Process_UART_Data(void)
{
	unsigned char i,k;
	unsigned int j;

	Para *param=(Para *)Parameters;

	if(UART_RX_Data[2]==CR)
	{
		tx_OK();
		return true;
	}
	else if(UART_RX_Data[4]==CR)				/* Read Parameters */
		i=0;
	else if(UART_RX_Data[4]=='=')		/* Set Parameters */
		i=1;
	else
		return false;

	if((UART_RX_Data[2]=='G')&&(UART_RX_Data[3]=='A'))
	{
		if(i)				/* Set Gateway IP */
		{
			if(parse_data()==true)
			{
				param->GATEWAY[0]=temporary[0];
				param->GATEWAY[1]=temporary[1];
				param->GATEWAY[2]=temporary[2];
				param->GATEWAY[3]=temporary[3];

				tx_OK();
			
				Erase_Page();
				Write_FALSH_Data();
			}
			else
				return false;
		}
		else				/* Read Gateway IP */
		{
			temporary[0]=param->GATEWAY[0];
			temporary[1]=param->GATEWAY[1];
			temporary[2]=param->GATEWAY[2];
			temporary[3]=param->GATEWAY[3];
			
			tx_data();
		}
	}
	else if(UART_RX_Data[2]=='S')
	{
		if(UART_RX_Data[3]=='M')
		{
			if(i)			/* Set Subnet mask */
			{
				if(parse_data()==true)
				{
					param->SUBNET[0]=temporary[0];
					param->SUBNET[1]=temporary[1];
					param->SUBNET[2]=temporary[2];
					param->SUBNET[3]=temporary[3];

					tx_OK();

					Erase_Page();
					Write_FALSH_Data();
				}
				else
					return false;
			}
			else			/* Read Subnet mask */
			{
				temporary[0]=param->SUBNET[0];
				temporary[1]=param->SUBNET[1];
				temporary[2]=param->SUBNET[2];
				temporary[3]=param->SUBNET[3];
			
				tx_data();
			}
		}
		else if(UART_RX_Data[3]=='R')
		{
			if(i)			/* Set Socket operation mode */
			{
				if(UART_RX_Data[6]==CR)
				{
					if((UART_RX_Data[5]>='0')&&(UART_RX_Data[5]<='2'))
					{
						param->Sn_MODE[0]=UART_RX_Data[5]-'0';

						tx_OK();

						Erase_Page();
						Write_FALSH_Data();
					}
					else
						return false;
				}
				else
					return false;
			}
			else			/* Read Socket operation mode */
			{
				UART_TX_Data[0]=0x30+((param->Sn_MODE[0])&0x0f);
				tx_delimiter(1);
			}
		}
		else
			return false;
	}

	else if(UART_RX_Data[2]=='P')
	{
		if(UART_RX_Data[3]=='H')				
		{
			if(i==0)		/* Read MAC address */
			{
				for(i=0;i<6;i++)
				{
					k=param->MAC_ADDR[i];
					k>>=4;
					if(k>9)
						k+=0x37;
					else
						k+=0x30;
					UART_TX_Data[2*i]=k;

					k=param->MAC_ADDR[i];
					k&=0x0f;
					if(k>9)
						k+=0x37;
					else
						k+=0x30;
					UART_TX_Data[2*i+1]=k;
				}

				tx_delimiter(12);
			}
			else
				return false;
		}
		else if(UART_RX_Data[3]=='T')				
		{
			if(i)			/* Set Socket port number */
			{
				if(parse_data1()==true)
				{
					param->Sn_SPORT[0][0]=temporary[0];
					param->Sn_SPORT[0][1]=temporary[1];

					tx_OK();

					Erase_Page();
					Write_FALSH_Data();
				}
				else
					return false;
			}
			else			/* Read Socket port number */
			{
				j=param->Sn_SPORT[0][0];
				j<<=8;
				j+=param->Sn_SPORT[0][1];

				tx_data1(j);
			}
		}
		else
			return false;
	}

	else if((UART_RX_Data[2]=='I')&&(UART_RX_Data[3]=='P'))
	{
		if(i)				/* Set IP address */
		{
			if(parse_data()==true)
			{
				param->IP[0]=temporary[0];
				param->IP[1]=temporary[1];
				param->IP[2]=temporary[2];
				param->IP[3]=temporary[3];

				tx_OK();

				Erase_Page();
				Write_FALSH_Data();
			}
			else
				return false;
		}
		else				/* Read IP address */
		{
			temporary[0]=param->IP[0];
			temporary[1]=param->IP[1];
			temporary[2]=param->IP[2];
			temporary[3]=param->IP[3];
			
			tx_data();
		}
	}
	else if(UART_RX_Data[2]=='D')
	{
		if(UART_RX_Data[3]=='A')
		{
			if(i)			/* Set Deationation IP Address */
			{
				if(parse_data()==true)
				{
					param->Sn_DIP[0][0]=temporary[0];
					param->Sn_DIP[0][1]=temporary[1];
					param->Sn_DIP[0][2]=temporary[2];
					param->Sn_DIP[0][3]=temporary[3];

					tx_OK();

					Erase_Page();
					Write_FALSH_Data();
				}
				else
					return false;
			}
			else			/* Read Deationation IP Address */
			{
				temporary[0]=param->Sn_DIP[0][0];
				temporary[1]=param->Sn_DIP[0][1];
				temporary[2]=param->Sn_DIP[0][2];
				temporary[3]=param->Sn_DIP[0][3];
			
				tx_data();
			}
		}
		else if(UART_RX_Data[3]=='P')
		{
			if(i)			/* Set Deationation Port number */
			{
				if(parse_data1()==true)
				{
					param->Sn_DPORT[0][0]=temporary[0];
					param->Sn_DPORT[0][1]=temporary[1];

					tx_OK();

					Erase_Page();
					Write_FALSH_Data();
				}
				else
					return false;
			}
			else			/* Read Deationation Port number */
			{
				j=param->Sn_DPORT[0][0];
				j<<=8;
				j+=param->Sn_DPORT[0][1];

				tx_data1(j);
			}
		}
		else
			return false;
	}
	else
		return false;

	return true;
}

/**************************************************************************************
if received UART DATA, Close SOCKET0 and keep UART communication 
**************************************************************************************/
void UART_Process(void)
{
	Close_Socket(0);
	UART_TXOK=1;

	do
	{
		if(UART_RX==1)
		{
			if(UART_RX_idx==1)
			{
				if(UART_RX_Data[0]!='A')
					UART_RX_idx=0;
			}
			else if(UART_RX_idx==2)
			{
				if(UART_RX_Data[1]!='T')
					UART_RX_idx=0;
			}
			else
			{
				if(UART_RX_Data[UART_RX_idx-1]==CR)
				{
					Delay(5);
					if(Process_UART_Data()==true)
					{
						while(UART_TXOK==0);
						UART_TXOK=0;
						
						UART_CMD=0;
						UART_TX_idx=0;
						SBUF0=LF;
					}
					else
						SBUF0=LF;

					UART_RX_idx=0;
				}
			}

			UART_RX=0;
		}
	}while(1);
}

unsigned char Check_AT(void)
{
	if(UART_RX_idx==1)
	{
		if(UART_RX_Data[0]!='A')
			UART_RX_idx=0;

		UART_RX=0;
		return false;
	}
	else if(UART_RX_idx==2)
	{
		if(UART_RX_Data[1]!='T')
			UART_RX_idx=0;

		UART_RX=0;
		return false;
	}
	else
	{
		if(UART_RX_Data[2]!=CR)
		{
			UART_RX_idx=0;
			UART_RX=0;
			return false;
		}
		else
			return true;
	}
}

/***************************************************************************************
Check Destination IP and PORT number in UDP mode
***************************************************************************************/
void Check_UDP_Destination(void)
{
	unsigned char xdata *ptr;
	unsigned char i;

	Para *param=(Para *)Parameters;
	
	ptr=(unsigned char xdata*)Sn_DIPR0(0);
	for(i=0;i<4;i++)
	{
		if(param->Sn_DIP[0][i]!=*ptr)
		{		
			ptr=(unsigned char xdata*)Sn_CR1(0);	/* Close SOCKETn */
			*ptr=Sn_CR_CLOSE;
			SOCKET_UDP(0);
			return;
		}
		ptr++;
	}
	
	ptr=(unsigned char xdata*)Sn_DPORTR0(0);
	for(i=0;i<2;i++)
	{
		if(param->Sn_DPORT[0][i]!=*ptr)
		{		
			ptr=(unsigned char xdata*)Sn_CR1(0);	/* Close SOCKETn */
			*ptr=Sn_CR_CLOSE;
			SOCKET_UDP(0);
			return;
		}
		ptr++;
	}
}

/***************************************************************************************
                                    Main program
***************************************************************************************/
void main(void)
{
	unsigned int i;

	Para *param=(Para *)Parameters;

	C8051F340_Initialization();
	RST_W5300=0;		/* Hardware Reset W5300 */

	Load_Parameters();

	Delay(5);
	RST_W5300=1;

	while(L_LINK)				/* Wait Ethernet LINK */
	{
		if(UART_RX==1)
		{
			if(Check_AT()==true)
				UART_Process();
		}
	};

	W5300_Init();					/* Initialize W5300 */
	
	if(Detect_Gateway()==true)
		D_OUTPUT1=0;				/* Detect Gateway */

/************ the following process only use SOCKET0 *******************/
/******* The usage of other SOCKET is the same as SOCKET0 ********/
	while(1)
	{
		if(S0_Init==0)
		{
			S0_Init=1;
			S0_SendOK=1;
			if(param->Sn_MODE[0]==TCP_SERVER)
			{
				if(SOCKET_Listen(0)==false)
					S0_Init=0;
			}
			else if(param->Sn_MODE[0]==TCP_CLIENT)
			{
				if(SOCKET_Connect(0)==false)
					S0_Init=0;
			}
			else
			{
				if(SOCKET_UDP(0)==false)
					S0_Init=0;
				else
					S0_Connect=1;
			}
		}
		
		if(S0_Receive)		/* If SOCKET0 receive DATA from peer SOCKET and Sned Data OK */
		{
			S0_Receive=0;
			S0_SendOK=0;

			i=Rx_Process(0);

			if(param->Sn_MODE[0]==UDP_MODE)
				Check_UDP_Destination();
			Tx_Process(0,i);

		}

		/* Process UART Data */
		if(UART_RX==1)
		{
			if(Check_AT()==true)
				UART_Process();
		}
	}
}
