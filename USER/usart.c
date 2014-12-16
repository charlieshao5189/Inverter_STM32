#include "usart.h"
#include <stdio.h>


void Uart1_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    /* config USART1 clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);


    /* USART1 GPIO config */
    /* Configure USART1 Tx (PA.09) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /* Configure USART1 Rx (PA.10) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* USART1 mode config */
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//使能接收中断
    USART_Cmd(USART1, ENABLE);

}

void Uart1_Send_Char(unsigned char tx_data)
{
    USART_SendData(USART1, tx_data);
}
//发送数组
void Uart1_Send_Char_Array(unsigned char *at, unsigned char length)
{
    {
        unsigned char i = 0;
        for(i = 0; i < length; i++)
            Uart1_Send_Char(at[i]);
    }
}
//发送字符串
void Uart1_Send_String(unsigned char *str)
{
    while(*str != '\0')
    {
        Uart1_Send_Char(*str);
        str++;
    }
}

/*
 * 函数名：fputc
 * 描述  ：重定向c库函数printf到USART1
 * 输入  ：无
 * 输出  ：无
 * 调用  ：由printf调用
 */
int fputc(int ch, FILE *f)
{
    /* 将Printf内容发往串口 */
    USART_SendData(USART1, (unsigned char) ch);
    while (!(USART1->SR & USART_FLAG_TXE));

    return (ch);
}
