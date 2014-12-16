/*****************************************************************************
Copyright:   2014, Shanghai Toptech Tech. Co., Ltd.
File name:   SysTick.c
Description: SysTick 系统滴答时钟10us中断函数库,中断时间可自由配置，
 *           常用的有 1us 10us 1ms 中断。
Author:      CharlieShao
Version:     V1.0
Date:        2014/12/1
History:     无
*****************************************************************************/
#include "SysTick.h"

static __IO u32 TimingDelay;

/*
 * 函数名：SysTick_Init
 * 描述  ：启动系统滴答定时器 SysTick
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void SysTick_Init(void)
{
    //最大延时233ms,对AHB时钟8分频时最大延时1864ms
    /* SystemFrequency / 1000    1ms中断一次
     * SystemFrequency / 100000	 10us中断一次
     * SystemFrequency / 1000000 1us中断一次
     */
    //	if (SysTick_Config(SystemFrequency / 100000))	// ST3.0.0库版本
    if (SysTick_Config(SystemCoreClock / 1000))	// ST3.5.0库版本
    {
        /* Capture error */
        while (1);
    }
    // 关闭滴答定时器
    SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}


/*
 * 函数名：Delay_us
 * 描述  ：us延时程序,单位由SysTick_Init(void)决定
 * 输入  ：- nTime
 * 输出  ：无
 * 调用  ：
 *       ：外部调用
 */

void Delay(__IO u32 nTime)
{
    TimingDelay = nTime;
    //使能滴答定时器
    SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;

    while(TimingDelay != 0);
}


/*
 * 函数名：TimingDelay_Decrement
 * 描述  ：获取节拍程序
 * 输入  ：无
 * 输出  ：无
 * 调用  ：在 SysTick 中断函数 SysTick_Handler()调用
 */
void TimingDelay_Decrement(void)
{
    if (TimingDelay != 0x00)
    {
        TimingDelay--;
    }
}
/***********************END OF FILE***************************/
