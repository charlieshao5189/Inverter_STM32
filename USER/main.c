/*****************************************************************************
Copyright:   2014, Shanghai Toptech Tech. Co., Ltd.
File name:   main.c
Description: ����������ɸ����ֳ�ʼ��
Author:      CharlieShao
Version:     V1.0
Date:        2014/12/1
History:     ��
*****************************************************************************/
#include "stm32f10x.h"
#include "SysTick.h"
#include "led.h"
#include "exti.h"
#include "key.h"
#include "usart.h"
#include "timer.h"
#include "st7920.h"
#include "adc.h"
#include "sin_tab.h"

u16 soft_cnt = 0;
vu16 out_fqc = 10;                  //���Ƶ�ʣ�Ĭ�ϳ�ʼΪ10HZ
vu16 out_volt = 60;
vu16 pre_out_fqc;
vu16 volt_cmd = 0;          /* torque command value x carrier /4 */

vu16 sin_cut = 0;	      		 /* skip - read value for sine pointer */
vu16 sin_pt = 0;							/* angle */
vu16 sinpt_sum = 0;         /* sum of angle  */

volatile PWM_LIMIT pwm;

/***********************************************************************************
FunctionName: void pwm_estimator(void)
Description:  ռ�ձȼ��㺯��
Input:        ��
Output:       U��V��Wÿ��ͨ��ռ�ձȵ����ֵ
Return:       ��
***********************************************************************************/
void pwm_estimator(void)
{
    /* sine pointer sum <-- skip-read value + sine pointer sum*/
    sinpt_sum = sin_cut + sinpt_sum;

    /* sine pointer sum maximun value 23040 = 360 x 64 */
    if(sinpt_sum > 23040)
    {
        sinpt_sum = sinpt_sum - 23040;
    }

    /*sine pointer <-- (sine pointer sum +32 ) / 64 */
    sin_pt = (sinpt_sum + 32 ) >> 6; //0-360

		 /*���´��������ڲ��������Ƿ����*/
    //	volt_cmd=3600;//volt_out=380
    //	sin_pt  =89; //sin_tbl[89]=32767
    //		/* U phase output command */
    //	pwm.ui  = (s32)sin_tbl[sin_pt]*(s32)(volt_cmd*2)>>16;    //3599=0x0E0F
    //	pwm.ui  = (C_CYCLE/2) - pwm.ui;      // voltage command  //1= 0001
    //
    //	volt_cmd=3600;//volt_out=380
    //	sin_pt  =269; //sin_tbl[269]=-32767

    //	pwm.ui  = (s32)sin_tbl[sin_pt]*(s32)(volt_cmd*2)>>16;    //-3599=0xFFFFF1F1
    //	pwm.ui  = (C_CYCLE/2) - pwm.ui;      // voltage command   //7199= 0x1C1F

    /* U phase output command */
    pwm.ui  = (s32)sin_tbl[sin_pt] * (s32)(volt_cmd * 2) >> 16;
    pwm.ui  = (C_CYCLE / 2) - pwm.ui;    // voltage command

    /* V phase output command */
    pwm.vi  = (s32)sin_tbl[sin_pt + 120] * (s32)(volt_cmd * 2) >> 16; ;
    pwm.vi =  (C_CYCLE / 2) - pwm.vi;   // voltage command

    /* W phase output command */
    pwm.wi = (s32)sin_tbl[sin_pt + 240] * (s32)(volt_cmd * 2) >> 16;
    pwm.wi = (C_CYCLE / 2) - pwm.wi;         // voltage command


    /* update buffer */
    pwm.uo = pwm.ui;
    pwm.vo = pwm.vi;
    pwm.wo = pwm.wi;

}


/***********************************************************************************
FunctionName: void set_parameter(void)
Description:  ���ò���
Input:        ��
Output:       sin_cut:�����������X64;volt_cmd:V/Fֵ
Return:       ��
***********************************************************************************/
void set_parameter(void)
{
    /*��Сֵ����*/
    //	 out_fqc= 10;
    // 	 out_volt= 60;
    //	/*skip-read valout_volte = 23040 * output frquency / sample frequency  1s����23040 * output frquency���㣬�������е�10000����*/
    //	sin_cut = (u16)(((u32)out_fqc*23040) / 10000);		//23= 0x0017
    //	/* voltage command value * carrier /4 */
    //	volt_cmd = (((u32)out_volt*(C_CYCLE/2))/380);     //568=0x0238
    //
    /*���ֵ����*/
    //	 out_fqc= 50;
    // 	 out_volt= 380;
    //	/*skip-read valout_volte = 23040 * output frquency / sample frequency  1s����23040 * output frquency���㣬�������е�10000����*/
    //	sin_cut = (u16)(((u32)out_fqc*23040) / 10000);		//115= 0x0073
    //	/* voltage command value * carrier /4 */
    //	volt_cmd = (((u32)out_volt*(C_CYCLE/2))/380);     // 3600= 0x0E10

    /*skip-read valout_volte = 23040 * output frquency / sample frequency  1s����23040 * output frquency���㣬�������е�10000����*/
    sin_cut = (u16)(((u32)out_fqc * 23040) / 10000);
    /* voltage command value * carrier /4 */
    volt_cmd = (((u32)out_volt * (C_CYCLE / 2)) / 380);
}
/*************************************************
FunctionName: void Key_Value_Deal(unsigned int key_value)
Description:  ����������
Input:        ��ֵ
Output:       ��
Return:       ��
*************************************************/
void Key_Value_Deal(unsigned int key_value)
{

    switch(key_value)
    {
    case  KEY_UP_Value :
    {
        if(out_fqc >= 50)
        {
            out_fqc = 49;
        }
        out_fqc++;
        out_volt = 7 * out_fqc + 30;
        GUI_DISP_HZ(1, 2, "��ǰƵ�ʣ�   Hz ");
        GUI_DISP_88_NUN_NEW(6, 2, out_fqc);
        GUI_DISP_HZ(1, 3, "���۵�ѹ��   V  ");
        GUI_DISP_888_NUN(6, 3, out_volt, 0);
        set_parameter();//ˢ��һ�²�������ͷ�ֵ
    };
    break; //
    case  KEY_DOWN_Value :
    {
        if(out_fqc <= 10)
        {
            out_fqc = 11;
        }
        out_fqc--;
        out_volt =  7 * out_fqc + 30;
        GUI_DISP_HZ(1, 2, "��ǰƵ�ʣ�   Hz ");
        GUI_DISP_88_NUN_NEW(6, 2, out_fqc);
        GUI_DISP_HZ(1, 3, "���۵�ѹ��   V  ");
        GUI_DISP_888_NUN(6, 3, out_volt, 0);
        set_parameter();//ˢ��һ�²�������ͷ�ֵ
    };
    break;
    case  KEY_ENTER_Value :;
        break; //
    case  KEY_BACK_Value :;
        break; //
    default:;
        break;
    }
}

/*************************************************
FunctionName: void Delay_JDQ_Init(void)
Description:  ��ʼ���̵����˿ڲ�������ʱ�̵�������·��ʱ����
Input:        ��
Output:       ��
Return:       ��
*************************************************/
void Delay_JDQ_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_ResetBits(GPIOC, GPIO_Pin_12);	
}

/*************************************************
FunctionName: int main(void)
Description:  ������
Input:        ��
Output:       ��
Return:       �ɹ����з���1��ʧ�ܷ���0
*************************************************/
int main(void)
{
    Delay_JDQ_Init();    //�̵����Ͽ����������贮������·
	  WorkLED_GPIO_Config();//����ָʾ�Ƴ�ʼ������Ƭ������Ĭ��Ϊ�ߣ�����ָʾ����˵����Ƭ������
	  SysTick_Init();//�δ�ʱ����ʼ�����ж�����1ms����Ҫ������ʱ��ʱ	  
	
	  Delay(500);//�ȴ�12864�����ȶ�
	  GUI_LCD_Init();   //12864��ʾ����ʼ��	
    GUI_LCD_CLR(0x00);//����
	  GUI_DISP_HZ(2, 1, "̩�տ˱�Ƶ��");
		
	  /*��������˸���º��ͨ��ʱ�̵���*/
		WorkLED(ON);
	  GUI_DISP_HZ(2, 2, "��ʼ����3");
		Delay(500);//��ʱ500ms����ʱ2s
  	WorkLED(OFF);
	  GUI_DISP_HZ(2, 2, "��ʼ����2");
	  Delay(500);
	  WorkLED(ON);
	  GUI_DISP_HZ(2, 2, "��ʼ����1");
		Delay(500);
		WorkLED(OFF);
    GUI_DISP_HZ(2, 2, "��ʼ����ϣ�");	
	  Delay(500);
	  WorkLED(ON);
	  GPIO_SetBits(GPIOC, GPIO_Pin_12);	//�̵����պϣ���·�������裬��ʼ����
	  

   
    Key_GPIO_Config();          //������ʼ��
    SPWM_Config();       //SPWM�����������
    NVIC_Configuration();//�ж����ȼ�����
		
    out_volt = 7 * out_fqc + 30;//��V/F���߽����˲���
    GUI_DISP_HZ(1, 2, "��ǰƵ�ʣ�   Hz ");
    GUI_DISP_88_NUN_NEW(6, 2, out_fqc);
    GUI_DISP_HZ(1, 3, "���۵�ѹ��   V  ");
    GUI_DISP_888_NUN(6, 3, out_volt, 0);
    GUI_DISP_HZ(2, 1, "̩�տ˱�Ƶ��");

    while(1)
    {
        Key_Value_Deal(Key_Value_Get());//����������
		}

}



/************************END OF FILE**************************/
