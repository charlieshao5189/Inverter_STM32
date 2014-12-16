#include "key.h"
#include "SysTick.h"

unsigned int key_value_past = 0;
/*************************************************
FunctionName: void Key_GPIO_Config(void)
Description:  按键GPIO口设置
Input:        无
Output:       无
Return:       无
*************************************************/
void Key_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOE, ENABLE);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

}


/*************************************************
FunctionName: unsigned int Key_Scan(void)
Description:  一次键值扫描
Input:        无
Output:       无
Return:       键值
*************************************************/
unsigned int Key_Scan(void)
{

    unsigned int temp_value = 0;
    unsigned int key_value  = 0;

    if(!KEY_ENTER)
    {
        temp_value |= 0x0001;
    }
    if(!KEY_UP )
    {
        temp_value |= 0x0002;
    }
    if(!KEY_DOWN)
    {
        temp_value |= 0x0004;
    }
    if(!KEY_BACK)
    {
        temp_value |= 0x0008;
    }
    //可以判断单按键的情况，也可判断组合键
    switch(temp_value)
    {
    case 0x0001 :
        key_value = 1;
        break; //
    case 0x0002 :
        key_value = 2;
        break; //
    case 0x0004 :
        key_value = 3;
        break; //
    case 0x0008 :
        key_value = 4;
        break; //
    default:
        key_value = 0;
        break;
    }
    return key_value;
}
/*************************************************
FunctionName: unsigned int Key_Value_Get()
Description:  获得可靠键值，并输出
Input:        无
Output:       无
Return:       键值，键值为0时说明没有按键按下
*************************************************/
unsigned int Key_Value_Get()
{
    unsigned char i;
    unsigned int key_value = 0;
    unsigned int key_value_buf = 0;

    key_value = Key_Scan();
    if(key_value != 0)
    {
        for(i = 0; i < 3; i++) //键值获取函数要放在主函数中，主函数中不能加延时程序，故重复三次消抖
        {
            key_value_buf = key_value;
            key_value = Key_Scan();
            if(key_value != key_value_buf)
            {
                key_value_past = 0;
                return 0;
            }
        }
        if(key_value != key_value_past)
        {
            //确实有按键按下并且与前一次按键不同
            key_value_past = key_value;
            return key_value;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        key_value_past = 0;
        return 0;
    }
}
