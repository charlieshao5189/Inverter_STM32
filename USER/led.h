#ifndef __LED_H
#define	__LED_H

#include "stm32f10x.h"

/* the macro definition to trigger the led on or off
 * 1 - off
 - 0 - on
 */
#define ON  0
#define OFF 1

#define WorkLED(a)	if (a)	\
					GPIO_SetBits(GPIOE,GPIO_Pin_1);\
					else		\
					GPIO_ResetBits(GPIOE,GPIO_Pin_1)

//#define LED2(a)	if (a)	\
//					GPIO_SetBits(GPIOC,GPIO_Pin_4);\
//					else		\
//					GPIO_ResetBits(GPIOC,GPIO_Pin_4)

//#define LED3(a)	if (a)	\
//					GPIO_SetBits(GPIOC,GPIO_Pin_5);\
//					else		\
//					GPIO_ResetBits(GPIOC,GPIO_Pin_5)

void WorkLED_GPIO_Config(void);

#endif /* __LED_H */
