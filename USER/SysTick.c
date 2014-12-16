/*****************************************************************************
Copyright:   2014, Shanghai Toptech Tech. Co., Ltd.
File name:   SysTick.c
Description: SysTick ϵͳ�δ�ʱ��10us�жϺ�����,�ж�ʱ����������ã�
 *           ���õ��� 1us 10us 1ms �жϡ�
Author:      CharlieShao
Version:     V1.0
Date:        2014/12/1
History:     ��
*****************************************************************************/
#include "SysTick.h"

static __IO u32 TimingDelay;

/*
 * ��������SysTick_Init
 * ����  ������ϵͳ�δ�ʱ�� SysTick
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void SysTick_Init(void)
{
    //�����ʱ233ms,��AHBʱ��8��Ƶʱ�����ʱ1864ms
    /* SystemFrequency / 1000    1ms�ж�һ��
     * SystemFrequency / 100000	 10us�ж�һ��
     * SystemFrequency / 1000000 1us�ж�һ��
     */
    //	if (SysTick_Config(SystemFrequency / 100000))	// ST3.0.0��汾
    if (SysTick_Config(SystemCoreClock / 1000))	// ST3.5.0��汾
    {
        /* Capture error */
        while (1);
    }
    // �رյδ�ʱ��
    SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}


/*
 * ��������Delay_us
 * ����  ��us��ʱ����,��λ��SysTick_Init(void)����
 * ����  ��- nTime
 * ���  ����
 * ����  ��
 *       ���ⲿ����
 */

void Delay(__IO u32 nTime)
{
    TimingDelay = nTime;
    //ʹ�ܵδ�ʱ��
    SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;

    while(TimingDelay != 0);
}


/*
 * ��������TimingDelay_Decrement
 * ����  ����ȡ���ĳ���
 * ����  ����
 * ���  ����
 * ����  ���� SysTick �жϺ��� SysTick_Handler()����
 */
void TimingDelay_Decrement(void)
{
    if (TimingDelay != 0x00)
    {
        TimingDelay--;
    }
}
/***********************END OF FILE***************************/
