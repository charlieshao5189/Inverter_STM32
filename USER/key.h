#ifndef __KEY_H
#define	__KEY_H

#include "stm32f10x.h"

#ifdef  DeBug

#define GPIO_KEY_ENTER       GPIOE
#define GPIO_Pin_KEY_ENTER   GPIO_Pin_1

#define GPIO_KEY_UP          GPIOB
#define GPIO_Pin_KEY_UP      GPIO_Pin_9

#define GPIO_KEY_DOWN        GPIOE
#define GPIO_Pin_KEY_DOWN    GPIO_Pin_0

#define GPIO_KEY_BACK        GPIOB
#define GPIO_Pin_KEY_BACK    GPIO_Pin_8

#define RCC_KEY_GPIO         RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOB 

#else

#define GPIO_KEY_ENTER       GPIOE
#define GPIO_Pin_KEY_ENTER   GPIO_Pin_7

#define GPIO_KEY_UP          GPIOE
#define GPIO_Pin_KEY_UP      GPIO_Pin_9

#define GPIO_KEY_DOWN        GPIOE
#define GPIO_Pin_KEY_DOWN    GPIO_Pin_8

#define GPIO_KEY_BACK        GPIOE
#define GPIO_Pin_KEY_BACK    GPIO_Pin_2

#define RCC_KEY_GPIO     RCC_APB2Periph_GPIOE

#endif

#define KEY_ENTER GPIO_ReadInputDataBit(GPIO_KEY_ENTER,GPIO_Pin_KEY_ENTER)
#define KEY_UP    GPIO_ReadInputDataBit(GPIO_KEY_UP,GPIO_Pin_KEY_UP)
#define KEY_DOWN  GPIO_ReadInputDataBit(GPIO_KEY_DOWN ,GPIO_Pin_KEY_DOWN)
#define KEY_BACK  GPIO_ReadInputDataBit(GPIO_KEY_BACK ,GPIO_Pin_KEY_BACK)

#define KEY_ENTER_Value  1
#define KEY_UP_Value     2
#define KEY_DOWN_Value   3
#define KEY_BACK_Value   4

void Key_GPIO_Config(void);
unsigned int Key_Value_Get(void);
#endif /* __KEY_H */
