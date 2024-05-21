#include<c8051F340.h>

#include"W5300.h"
#include"Parameters.h"
#include"Device1.h"

/* Close SOCKETn */
void Close_Socket(SOCKET s)
{
	unsigned char xdata *ptr;

	ptr=(unsigned char xdata*)Sn_MR1(s);	/* Check SOCKETn operation mode */
	if(*ptr==Sn_MR_TCP)
	{
		ptr=(unsigned char xdata*)Sn_SSR1(s);	/* Set SOCKET0 in TCP mode */
		if(*ptr!=SOCK_ESTABLISHED)
		{
			ptr=(unsigned char xdata*)Sn_CR1(s);	/* Close SOCKETn */
			*ptr=Sn_CR_DISCON;
		}
	}

	ptr=(unsigned char xdata*)Sn_CR1(s);	/* Close SOCKETn */
	*ptr=Sn_CR_CLOSE;

}

/***********************************************************************************************
W5300 Interrupt process
***********************************************************************************************/
void W5300_Interrupt_Process(void)
{
	unsigned char xdata *ptr;
	unsigned char i,j;
	
	ptr=(unsigned char xdata*)IR0;
	i=*ptr;
	*ptr=i;
	if(i&IR_CONFLICT)
	{
		/* Process IP Conflict */
	}
	if(i&IR_UNREACH)
	{
		/* Process Destionation unreach */
	}
	if(i&IR_PPPoE)
	{
		/* Process PPPoE Termination */
	}
	if(i&IR_FMTU)
	{
		/* Receiving ICMP packet */
	}

	ptr=(unsigned char xdata*)IR1;
	i=*ptr;
	*ptr=i;

	if(i&IR_SOCK(0))			/* Socket0 */
	{
		ptr=(unsigned char xdata*)Sn_IR1(0);
		j=*ptr;
		*ptr=j;

		if(j&Sn_IR_SEND_OK)			/* Send data complete */
			S0_SendOK=1;
		if(j&Sn_IR_TIMEOUT)			/* Timeout */
		{
			S0_Timeout=1;
			S0_Connect=0;
			S0_Init=0;
		}
		if(j&Sn_IR_RECV)			/* Receive data from peer socket */
			S0_Receive=1;
		if(j&Sn_IR_CON)				/* Make connection with peer socket */
			S0_Connect=1;
		if(j&Sn_IR_DISCON)			/* disconnect with peer socket */
		{
			S0_Connect=0;
			S0_Init=0;
		}
	}

	if(i&IR_SOCK(1))			/* Socket1 */
	{
		ptr=(unsigned char xdata*)Sn_IR1(1);
		j=*ptr;
		*ptr=j;

		if(j&Sn_IR_SEND_OK)			/* Send data complete */
			S1_SendOK=1;
		if(j&Sn_IR_TIMEOUT)			/* Timeout */
		{
			S1_Timeout=1;
			S1_Connect=0;
		}
		if(j&Sn_IR_RECV)			/* Receive data from peer socket */
			S1_Receive=1;
		if(j&Sn_IR_CON)				/* Make connection with peer socket */
			S1_Connect=1;
		if(j&Sn_IR_DISCON)			/* disconnect with peer socket */
			S1_Connect=0;
	}

	if(i&IR_SOCK(2))			/* Socket2 */
	{
		ptr=(unsigned char xdata*)Sn_IR1(2);
		j=*ptr;
		*ptr=j;

		if(j&Sn_IR_SEND_OK)			/* Send data complete */
			S2_SendOK=1;
		if(j&Sn_IR_TIMEOUT)			/* Timeout */
		{
			S2_Timeout=1;
			S2_Connect=0;
		}
		if(j&Sn_IR_RECV)			/* Receive data from peer socket */
			S2_Receive=1;
		if(j&Sn_IR_CON)				/* Make connection with peer socket */
			S2_Connect=1;
		if(j&Sn_IR_DISCON)			/* disconnect with peer socket */
			S2_Connect=0;
	}

	if(i&IR_SOCK(3))			/* Socket3 */
	{
		ptr=(unsigned char xdata*)Sn_IR1(3);
		j=*ptr;
		*ptr=j;

		if(j&Sn_IR_SEND_OK)			/* Send data complete */
			S3_SendOK=1;
		if(j&Sn_IR_TIMEOUT)			/* Timeout */
		{
			S3_Timeout=1;
			S3_Connect=0;
		}
		if(j&Sn_IR_RECV)			/* Receive data from peer socket */
			S3_Receive=1;
		if(j&Sn_IR_CON)				/* Make connection with peer socket */
			S3_Connect=1;
		if(j&Sn_IR_DISCON)			/* disconnect with peer socket */
			S3_Connect=0;
	}

	if(i&IR_SOCK(4))			/* Socket4 */
	{
		ptr=(unsigned char xdata*)Sn_IR1(4);
		j=*ptr;
		*ptr=j;

		if(j&Sn_IR_SEND_OK)			/* Send data complete */
			S4_SendOK=1;
		if(j&Sn_IR_TIMEOUT)			/* Timeout */
		{
			S4_Timeout=1;
			S4_Connect=0;
		}
		if(j&Sn_IR_RECV)			/* Receive data from peer socket */
			S4_Receive=1;
		if(j&Sn_IR_CON)				/* Make connection with peer socket */
			S4_Connect=1;
		if(j&Sn_IR_DISCON)			/* disconnect with peer socket */
			S4_Connect=0;
	}

	if(i&IR_SOCK(5))			/* Socket5 */
	{
		ptr=(unsigned char xdata*)Sn_IR1(5);
		j=*ptr;
		*ptr=j;

		if(j&Sn_IR_SEND_OK)			/* Send data complete */
			S5_SendOK=1;
		if(j&Sn_IR_TIMEOUT)			/* Timeout */
		{
			S5_Timeout=1;
			S5_Connect=0;
		}
		if(j&Sn_IR_RECV)			/* Receive data from peer socket */
			S5_Receive=1;
		if(j&Sn_IR_CON)				/* Make connection with peer socket */
			S5_Connect=1;
		if(j&Sn_IR_DISCON)			/* disconnect with peer socket */
			S5_Connect=0;
	}

	if(i&IR_SOCK(6))			/* Socket6 */
	{
		ptr=(unsigned char xdata*)Sn_IR1(6);
		j=*ptr;
		*ptr=j;

		if(j&Sn_IR_SEND_OK)			/* Send data complete */
			S6_SendOK=1;
		if(j&Sn_IR_TIMEOUT)			/* Timeout */
		{
			S6_Timeout=1;
			S6_Connect=0;
		}
		if(j&Sn_IR_RECV)			/* Receive data from peer socket */
			S6_Receive=1;
		if(j&Sn_IR_CON)				/* Make connection with peer socket */
			S6_Connect=1;
		if(j&Sn_IR_DISCON)			/* disconnect with peer socket */
			S6_Connect=0;
	}

	if(i&IR_SOCK(7))			/* Socket0 */
	{
		ptr=(unsigned char xdata*)Sn_IR1(7);
		j=*ptr;
		*ptr=j;

		if(j&Sn_IR_SEND_OK)			/* Send data complete */
			S7_SendOK=1;
		if(j&Sn_IR_TIMEOUT)			/* Timeout */
		{
			S7_Timeout=1;
			S7_Connect=0;
		}
		if(j&Sn_IR_RECV)			/* Receive data from peer socket */
			S7_Receive=1;
		if(j&Sn_IR_CON)				/* Make connection with peer socket */
			S7_Connect=1;
		if(j&Sn_IR_DISCON)			/* disconnect with peer socket */
			S7_Connect=0;
	}
}

/***********************************************************************************************
Initialize W5300, Such as Hardware address, Gateway address, Subnet mask and Source IP address
***********************************************************************************************/
void W5300_Init(void)
{
	unsigned char xdata *ptr;
	unsigned char i;

	Para *param=(Para *)Parameters;

	/* Software Reset */
	ptr=(unsigned char xdata*)MR1;
	*ptr=MR_RST;
	Delay(10);			/* Delay 100ms */
	
	ptr=(unsigned char xdata*)SHAR0;	/* Set Hardware address */
	for(i=0;i<6;i++)
	{
		*ptr=param->MAC_ADDR[i];
		ptr++;
	}
	
	ptr=(unsigned char xdata*)GAR0;		/* Set Gateway address */
	for(i=0;i<4;i++)
	{
		*ptr=param->GATEWAY[i];
		ptr++;
	}
	
	ptr=(unsigned char xdata*)SUBR0;	/* Set Subnet mask */
	for(i=0;i<4;i++)
	{
		*ptr=param->SUBNET[i];
		ptr++;
	}
	
	ptr=(unsigned char xdata*)SIPR0;	/* Set Source IP address */
	for(i=0;i<4;i++)
	{
		*ptr=param->IP[i];
		ptr++;
	}

	/*********************************************************** 
	Default memory type 
	MTYPER0=0;			memory block 15~8 set as RX memory
	MTYPER1=0xff;		memory block 7~0 set as TX memory
	user may change the value according to DATA communication
	***********************************************************/
	
/***************************************************************
****************** Default TX memory allocation ***************/
	ptr=(unsigned char xdata*)TMSR0;
	*ptr=0x02;			/* Set SOCKET0 TX memory size 2K */
	ptr++;
	*ptr=0x14;
	ptr++;
	*ptr=0x08;
	ptr++;
	*ptr=0x08;
	ptr++;
	*ptr=0x08;
	ptr++;
	*ptr=0x08;
	ptr++;
	*ptr=0x08;
	ptr++;
	*ptr=0x08;
/*	user may change the value according to DATA communication
***************************************************************/

/***************************************************************
***************** Default RX memory allocation ****************/
	ptr=(unsigned char xdata*)RMSR0;
	*ptr=0x02;				/* Set SOCKET0 RX memory size 2K */
	ptr++;
	*ptr=0x14;
	ptr++;
	*ptr=0x08;
	ptr++;
	*ptr=0x08;
	ptr++;
	*ptr=0x08;
	ptr++;
	*ptr=0x08;
	ptr++;
	*ptr=0x08;
	ptr++;
	*ptr=0x08;
/*   user may change the value according to DATA communication
*********************************************************************/

	ptr=(unsigned char xdata*)IMR0;		/* Set Source IP address */
	*ptr=IR_CONFLICT|IR_UNREACH;		/* IP conflict and IP unreach interrupt*/
	ptr++;
	*ptr=0xff;							/* SOCKET0~SOCKET7 interrupt */
}

/********************* Detect Gateway ********************************************
Use SOCKET0 to detect Gatewat. If don't use Gateway, this step can be skipped
Return TRUE if detecte Gateway
Return FALSE if no Gateway
*********************************************************************************/
unsigned char Detect_Gateway(void)
{
	unsigned char xdata *ptr;
	unsigned char i;

	Para *param=(Para *)Parameters;

	ptr=(unsigned char xdata*)Sn_MR1(0);	/* Set SOCKET0 in TCP mode */
	*ptr=Sn_MR_TCP;
	
	ptr=(unsigned char xdata*)Sn_CR1(0);	/* Open SOCKET0 */
	*ptr=Sn_CR_OPEN;

	ptr=(unsigned char xdata*)Sn_SSR1(0);	/* Set SOCKET0 in TCP mode */
	if(*ptr!=SOCK_INIT)
	{
		ptr=(unsigned char xdata*)Sn_CR1(0);	/* Close SOCKET0 */
		*ptr=Sn_CR_CLOSE;
		
		return false;
	}

	ptr=(unsigned char xdata*)Sn_DIPR0(0);	/* Set Destination IP address */
	for(i=0;i<4;i++)
	{
		*ptr=(param->IP[i])+1;				/* Set Destionation IP in the differnt subnet */
		ptr++;
	}
	
	ptr=(unsigned char xdata*)Sn_CR1(0);	/* Make connection*/
	*ptr=Sn_CR_CONNECT;
	
	ptr=(unsigned char xdata*)Sn_DHAR0(0);
	do
	{
		if(S0_Timeout)
		{
			S0_Timeout=0;
			break;
		}
		else
		{
			if(*ptr!=0xff)
				return true;
		}
	}while(1);

	return false;
}

/**************************************************************************
Set SOCKETn in TCP Server mode
Return TRUE if Set OK
Return FALSE if failed
**************************************************************************/
unsigned char SOCKET_Listen(SOCKET s)
{
	unsigned char xdata *ptr;

	Para *param=(Para *)Parameters;
	
	/* Set socket port number */
	ptr=(unsigned char xdata*)Sn_PORTR0(s);
	*ptr=param->Sn_SPORT[s][0];
	ptr++;
	*ptr=param->Sn_SPORT[s][1];
	
	ptr=(unsigned char xdata*)Sn_MR1(s);	/* Set SOCKETn in TCP mode */
	*ptr=Sn_MR_TCP;
	
	ptr=(unsigned char xdata*)Sn_CR1(s);	/* Open SOCKETn */
	*ptr=Sn_CR_OPEN;

	ptr=(unsigned char xdata*)Sn_SSR1(s);	/* Set SOCKETn in TCP mode */
	if(*ptr!=SOCK_INIT)
	{
		ptr=(unsigned char xdata*)Sn_CR1(s);	/* Close SOCKETn */
		*ptr=Sn_CR_CLOSE;
		
		return false;
	}

	ptr=(unsigned char xdata*)Sn_CR1(s);	/* Set Socketn in TCP Server*/
	*ptr=Sn_CR_LISTEN;

	ptr=(unsigned char xdata*)Sn_SSR1(s);	/* Set SOCKETn in TCP mode */
	if(*ptr!=SOCK_LISTEN)
	{
		ptr=(unsigned char xdata*)Sn_CR1(s);	/* Close SOCKETn */
		*ptr=Sn_CR_CLOSE;
		
		return false;
	}

	return true;
}

/**************************************************************************
Set SOCKETn in TCP Cient mode
Return TRUE if Set OK
Return FALSE if failed
**************************************************************************/
unsigned char SOCKET_Connect(SOCKET s)
{
	unsigned char xdata *ptr;

	Para *param=(Para *)Parameters;
	
	/* Set socket port number */
	ptr=(unsigned char xdata*)Sn_PORTR0(s);
	*ptr=param->Sn_SPORT[s][0];
	ptr++;
	*ptr=param->Sn_SPORT[s][1];
	
	/* Set SOCKET Destination IP Address */
	ptr=(unsigned char xdata*)Sn_DIPR0(s);
	*ptr=param->Sn_DIP[s][0];
	ptr++;
	*ptr=param->Sn_DIP[s][1];
	ptr++;
	*ptr=param->Sn_DIP[s][2];
	ptr++;
	*ptr=param->Sn_DIP[s][3];

	/* Set SOCKET Destination Port Number */
	ptr=(unsigned char xdata*)Sn_DPORTR0(s);
	*ptr=param->Sn_DPORT[s][0];
	ptr++;
	*ptr=param->Sn_DPORT[s][1];

	ptr=(unsigned char xdata*)Sn_MR1(s);	/* Set SOCKETn in TCP mode */
	*ptr=Sn_MR_TCP;
	
	ptr=(unsigned char xdata*)Sn_CR1(s);	/* Open SOCKETn */
	*ptr=Sn_CR_OPEN;

	ptr=(unsigned char xdata*)Sn_SSR1(s);	/* Set SOCKETn in TCP mode */
	if(*ptr!=SOCK_INIT)
	{
		ptr=(unsigned char xdata*)Sn_CR1(s);	/* Close SOCKETn */
		*ptr=Sn_CR_CLOSE;
		
		return false;
	}

	ptr=(unsigned char xdata*)Sn_CR1(s);	/* Make connection to peer SOCKET */
	*ptr=Sn_CR_CONNECT;

	return true;
}

/**************************************************************************
Set SOCKETn in UDP mode
Return TRUE if Set OK
Return FALSE if failed
**************************************************************************/
unsigned char SOCKET_UDP(SOCKET s)
{
	unsigned char xdata *ptr;

	Para *param=(Para *)Parameters;
	
	/* Set socket port number */
	ptr=(unsigned char xdata*)Sn_PORTR0(s);
	*ptr=param->Sn_SPORT[s][0];
	ptr++;
	*ptr=param->Sn_SPORT[s][1];
	
	/* Set SOCKET Destination IP Address */
	ptr=(unsigned char xdata*)Sn_DIPR0(s);
	*ptr=param->Sn_DIP[s][0];
	ptr++;
	*ptr=param->Sn_DIP[s][1];
	ptr++;
	*ptr=param->Sn_DIP[s][2];
	ptr++;
	*ptr=param->Sn_DIP[s][3];

	/* Set SOCKET Destination Port Number */
	ptr=(unsigned char xdata*)Sn_DPORTR0(s);
	*ptr=param->Sn_DPORT[s][0];
	ptr++;
	*ptr=param->Sn_DPORT[s][1];

	ptr=(unsigned char xdata*)Sn_MR1(s);	/* Set SOCKETn in TCP mode */
	*ptr=Sn_MR_UDP;
	
	ptr=(unsigned char xdata*)Sn_CR1(s);	/* Open SOCKETn */
	*ptr=Sn_CR_OPEN;

	ptr=(unsigned char xdata*)Sn_SSR1(s);	/* Set SOCKETn in UDP mode */
	if(*ptr!=SOCK_UDP)
	{
		ptr=(unsigned char xdata*)Sn_CR1(s);	/* Close SOCKETn */
		*ptr=Sn_CR_CLOSE;
		
		return false;
	}

	return true;
}

/*************************************************************************
SOCKETn received DATA process
Copy Data in Rx_Buffer
Return Data size
*************************************************************************/
unsigned int Rx_Process(SOCKET s)
{
	unsigned int size;		/* Assume that Data size is no more than 64K */
	unsigned int i,j;
	unsigned char xdata *ptr;

	Para *param=(Para *)Parameters;

	ptr=(unsigned char xdata*)Sn_RX_RSR2(s);	/* Read Data Size */
	size=*ptr;
	ptr++;
	size=(size<<8)+*ptr;
	
	ptr=(unsigned char xdata*)Sn_RX_FIFOR(s);
	if(param->Sn_MODE[0]!=UDP_MODE)
	{	/* TCP mode */
		j=size/2-1;

		size=*ptr;								/* Read the real size of the DATA packet */
		size<<=8;
		ptr++;
		size+=*ptr;
		ptr--;
	}
	else
	{	/* UDP mode */
		j=size/2-4;

		param->Sn_DIP[0][0]=*ptr;						/* Destination IP */
		ptr++;
		param->Sn_DIP[0][1]=*ptr;
		ptr--;
		param->Sn_DIP[0][2]=*ptr;
		ptr++;
		param->Sn_DIP[0][3]=*ptr;
		ptr--;
		
		param->Sn_DPORT[0][0]=*ptr;					/* Detionation Port number */
		ptr++;
		param->Sn_DPORT[0][1]=*ptr;
		ptr--;

		size=*ptr;								/* Read the real size of the DATA packet */
		size<<=8;
		ptr++;
		size+=*ptr;
		ptr--;
	}

	i=0;
	while(j!=0)									/* Copy Data to Tx_Buffer */
	{
		Data_Buffer[i]=*ptr;
		i++;
		ptr++;
		Data_Buffer[i]=*ptr;
		i++;
		ptr--;
		j--;
	}
	
	ptr=(unsigned char xdata*)Sn_CR1(s);		/* RECV command */
	*ptr=Sn_CR_RECV;

	return size;
}

/*************************************************************************
Transmit DATA through SOCKETn
Data in Tx_Buffer is to be transmited
Input: Data size
*************************************************************************/
unsigned char Tx_Process(SOCKET s, unsigned int size)
{
	unsigned int i,j;		/* Assume that Data size is no more than 64K */
	unsigned char xdata *ptr;
	
	Para *param=(Para *)Parameters;

	if(size>S_TX_SIZE)
		return false;

	if(size&0x0001)
		j=(size+1)/2;
	else
		j=size/2;

	ptr=(unsigned char xdata*)Sn_TX_FIFOR(s);
	i=0;
	while(j!=0)				/* Copy Tx_Buffer data to Tx memory */
	{
		*ptr=Data_Buffer[i];
		i++;
		ptr++;
		*ptr=Data_Buffer[i];
		i++;
		ptr--;
		j--;
	}
	
	ptr=(unsigned char xdata*)Sn_TX_WRSR2(s);	/* sets transmission data size to Sn_TX_WRSR */
	*ptr=size/256;
	ptr++;
	*ptr=size;

	ptr=(unsigned char xdata*)Sn_CR1(s);
	*ptr=Sn_CR_SEND;
}
