/*****************************************************************************
Copyright:   2014, Shanghai Toptech Tech. Co., Ltd.
File name:   led.c
Description: led应用函数
Author:      CharlieShao
Version:     V1.0
Date:        2014/12/1
History:     无
*****************************************************************************/
#include "led.h"
/***********************************************************************************
FunctionName: void WorkLED_GPIO_Config(void)
Description:  工作信号指示灯的初始化函数
Input:        无
Output:       无
Return:       无
***********************************************************************************/
void WorkLED_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    //点亮工作指示灯
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    GPIO_ResetBits(GPIOE, GPIO_Pin_1);	 // turn off all led
}


/************************END OF FILE************************/
