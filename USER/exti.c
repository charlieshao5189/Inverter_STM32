/*****************************************************************************
Copyright:   2014, Shanghai Toptech Tech. Co., Ltd.
File name:   exti.c
Description: 中断应用函数
Author:      CharlieShao
Version:     V1.0
Date:        2014/12/1
History:     无
*****************************************************************************/
#include "exti.h"

 /*************************************************
FunctionName: void NVIC_Configuration(void)
Description:  配置嵌套向量中断控制器NVIC
Input:        无
Output:       无
Return:       无
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
// * 函数名：EXTI_PE5_Config
// * 描述  ：配置 PE5 为线中断口，并设置中断优先级
// * 输入  ：无
// * 输出  ：无
// * 调用  ：外部调用
// */
//void EXTI_PE5_Config(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	EXTI_InitTypeDef EXTI_InitStructure;

//	/* config the extiline(PE5) clock and AFIO clock */
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO,ENABLE);
//	/* EXTI line gpio config(PE5) */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	 // 上拉输入
//  GPIO_Init(GPIOE, &GPIO_InitStructure);
//  /* config the NVIC(PE5) */
//	NVIC_Configuration();
//	/* EXTI line(PE5) mode config */
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource5);
//  EXTI_InitStructure.EXTI_Line = EXTI_Line5;
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure);
//}
/************************************END OF FILE**************************************/
