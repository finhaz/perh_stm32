#ifndef __INIT_CONFIG_H__
#define __INIT_CONFIG_H__
#include "stm32f10x.h"
void  Init_Config(void);
extern volatile unsigned char Uart1_rev_comflag;
/*u8 recenum;
u8 sendnum;*/
extern volatile unsigned char datenum;
extern volatile unsigned char datesave[16];
extern volatile unsigned char Uart1_rev_comflag8;
extern volatile unsigned char Uart1_rev_comflag11;
/*u8 aRxBuff[32];	            // 接收缓冲区
u8 aTxBuff[32];			//发送缓冲区*/
extern volatile unsigned char    nTxBuff;
extern void RCC_Configuration(void);
//extern void test(void);
#endif
