#ifndef __USART_H
#define	__USART_H

#include "stm32f10x.h"

void Uart1_Config(void);

void Uart1_Send_Char(unsigned char tx_data);
void Uart1_Send_Char_Array(unsigned char *at, unsigned char length);
void Uart1_Send_String(unsigned char *str);
unsigned char Uart_Receive_Char(void);

#endif /* __USART_H */
