#include "adc.h"
#include "st7920.h"

/*************************************************
FunctionName: void Display_ADC1_Value(void)
Description:  ��ʾADC1��ͨ����ת��ֵ
Input:        ��
Output:       ��
Return:       ��
*************************************************/
void Display_ADC1_Value(void)
{

    GUI_DISP_888_NUN(1, 4, ADC1_Get_Value(10), 0);
    GUI_DISP_888_NUN(3, 4, ADC1_Get_Value(11), 0);
    GUI_DISP_888_NUN(5, 4, ADC1_Get_Value(12), 0);

}

