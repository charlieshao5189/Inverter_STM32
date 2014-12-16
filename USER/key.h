#ifndef __KEY_H
#define	__KEY_H

#include "stm32f10x.h"

#define KEY_ENTER GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_7)
#define KEY_UP    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_9)
#define KEY_DOWN  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_8)
#define KEY_BACK  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2)

#define KEY_ENTER_Value  1
#define KEY_UP_Value     2
#define KEY_DOWN_Value   3
#define KEY_BACK_Value   4

void Key_GPIO_Config(void);
unsigned int Key_Value_Get(void);
#endif /* __KEY_H */
