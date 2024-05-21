#ifndef __UART1_H__
#define __UART1_H__

#include "stm32f10x.h"

void USART1_Configuration(void);
void usart_sent_char(USART_TypeDef* USARTx,u8 ch );
void usart_sent_string(USART_TypeDef* USARTx,char *ch);
#endif
