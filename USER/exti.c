/*****************************************************************************
Copyright:   2014, Shanghai Toptech Tech. Co., Ltd.
File name:   exti.c
Description: �ж�Ӧ�ú���
Author:      CharlieShao
Version:     V1.0
Date:        2014/12/1
History:     ��
*****************************************************************************/
#include "exti.h"

 /*************************************************
FunctionName: void NVIC_Configuration(void)
Description:  ����Ƕ�������жϿ�����NVIC
Input:        ��
Output:       ��
Return:       ��
*************************************************/
void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Configure the NVIC Preemption Priority Bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

    //	/* Enable the USARTy Interrupt */
    //	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    //	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    //	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    //	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    //	NVIC_Init(&NVIC_InitStructure);


	  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init( &NVIC_InitStructure );
	
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init( &NVIC_InitStructure );
		
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init( &NVIC_InitStructure );

   


}

///*
// * ��������EXTI_PE5_Config
// * ����  ������ PE5 Ϊ���жϿڣ��������ж����ȼ�
// * ����  ����
// * ���  ����
// * ����  ���ⲿ����
// */
//void EXTI_PE5_Config(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	EXTI_InitTypeDef EXTI_InitStructure;

//	/* config the extiline(PE5) clock and AFIO clock */
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO,ENABLE);
//	/* EXTI line gpio config(PE5) */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	 // ��������
//  GPIO_Init(GPIOE, &GPIO_InitStructure);
//  /* config the NVIC(PE5) */
//	NVIC_Configuration();
//	/* EXTI line(PE5) mode config */
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource5);
//  EXTI_InitStructure.EXTI_Line = EXTI_Line5;
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½����ж�
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure);
//}
/************************************END OF FILE**************************************/
