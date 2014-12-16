/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "timer.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */

extern void TimingDelay_Decrement(void);
void SysTick_Handler(void)//1ms进来一次
{
    TimingDelay_Decrement();
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */
//void USART1_IRQHandler(void)
//{
//	unsigned char c;
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
//	{
//	    c=USART1->DR;
//	  	USART_SendData(USART1,c);//将接受到的数据直接返回打印
//	}
//
//}

 /*************************************************
FunctionName: void TIM1_UP_IRQHandler(void)
Description:  定时器中断处理函数，每次定时器发生溢出终端，更新占空比
Input:        无
Output:       无
Return:       无
*************************************************/
extern  PWM_LIMIT pwm;

void TIM1_UP_IRQHandler(void)
{
    if( TIM_GetITStatus( TIM1, TIM_IT_Update ) != RESET )
    {
        TIM1->CCR1 = pwm.uo;
        TIM1->CCR2 = pwm.vo;
        TIM1->CCR3 = pwm.wo;
        TIM_ClearITPendingBit( TIM1, TIM_IT_Update );
    }
}



/*************************************************
FunctionName: void TIM1_UP_IRQHandler(void)
Description:  This function handles TIM2 global interrupt request.100us进来一次，计算并输出占空比即幅值
Input:        无
Output:       无
Return:       无
*************************************************/
extern void pwm_estimator(void);

void TIM2_IRQHandler(void)
{
    if( TIM_GetITStatus( TIM2, TIM_IT_Update ) != RESET )
    {
        pwm_estimator();
        TIM_ClearITPendingBit( TIM2, TIM_IT_Update );
    }
}


/*************************************************
FunctionName: void TIM3_IRQHandler(void)
Description:  0.2ms进来一次，连续按键处理
Input:        无
Output:       无
Return:       无
*************************************************/
extern unsigned int key_value_past;
extern void Key_Value_Deal(unsigned int key_value);
static unsigned int time3_count = 0;

void TIM3_IRQHandler(void)
{
    if( TIM_GetITStatus( TIM3, TIM_IT_Update ) != RESET )
    {
        time3_count++;
        if(time3_count > 1000)
        {
            time3_count = 0;
        }
        if((key_value_past != 0) && (time3_count == 1000))
        {
            if(key_value_past == 2)
            {
                Key_Value_Deal(key_value_past);
            }
            else if(key_value_past == 3)
            {
                Key_Value_Deal(key_value_past);
            }
        }
//        if(time3_count == 1000)
//        {
//            Display_ADC1_Value();
//        }
        TIM_ClearITPendingBit( TIM3, TIM_IT_Update );
    }
}
/***********************END OF FILE************************/
