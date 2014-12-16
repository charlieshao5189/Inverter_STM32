#include "key.h"
#include "SysTick.h"

unsigned int key_value_past = 0;
/*************************************************
FunctionName: void Key_GPIO_Config(void)
Description:  ����GPIO������
Input:        ��
Output:       ��
Return:       ��
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
Description:  һ�μ�ֵɨ��
Input:        ��
Output:       ��
Return:       ��ֵ
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
    //�����жϵ������������Ҳ���ж���ϼ�
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
Description:  ��ÿɿ���ֵ�������
Input:        ��
Output:       ��
Return:       ��ֵ����ֵΪ0ʱ˵��û�а�������
*************************************************/
unsigned int Key_Value_Get()
{
    unsigned char i;
    unsigned int key_value = 0;
    unsigned int key_value_buf = 0;

    key_value = Key_Scan();
    if(key_value != 0)
    {
        for(i = 0; i < 3; i++) //��ֵ��ȡ����Ҫ�����������У��������в��ܼ���ʱ���򣬹��ظ���������
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
            //ȷʵ�а������²�����ǰһ�ΰ�����ͬ
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
