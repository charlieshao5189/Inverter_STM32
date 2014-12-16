#include "adc.h"
#include "st7920.h"

/*************************************************
FunctionName: void Display_ADC1_Value(void)
Description:  显示ADC1各通道的转换值
Input:        无
Output:       无
Return:       无
*************************************************/
void Display_ADC1_Value(void)
{

    GUI_DISP_888_NUN(1, 4, ADC1_Get_Value(10), 0);
    GUI_DISP_888_NUN(3, 4, ADC1_Get_Value(11), 0);
    GUI_DISP_888_NUN(5, 4, ADC1_Get_Value(12), 0);

}

