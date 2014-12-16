#ifndef __ADC_H
#define	__ADC_H

#include "stm32f10x.h"

void ADC1_Init(void);
unsigned int ADC1_Get_Value(unsigned char Chinel_num);



#endif /* __ADC_H */
