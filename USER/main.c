/*****************************************************************************
Copyright:   2014, Shanghai Toptech Tech. Co., Ltd.
File name:   main.c
Description: 主函数，完成各部分初始化
Author:      CharlieShao
Version:     V1.0
Date:        2014/12/1
History:     无
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
vu16 out_fqc = 10;                  //输出频率，默认初始为10HZ
vu16 out_volt = 60;
vu16 pre_out_fqc;
vu16 volt_cmd = 0;          /* torque command value x carrier /4 */

vu16 sin_cut = 0;	      		 /* skip - read value for sine pointer */
vu16 sin_pt = 0;							/* angle */
vu16 sinpt_sum = 0;         /* sum of angle  */

volatile PWM_LIMIT pwm;

/***********************************************************************************
FunctionName: void pwm_estimator(void)
Description:  占空比计算函数
Input:        无
Output:       U、V、W每个通道占空比的输出值
Return:       无
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

		 /*以下代码曾用于测试数据是否溢出*/
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
Description:  设置参数
Input:        无
Output:       sin_cut:采样间隔度数X64;volt_cmd:V/F值
Return:       无
***********************************************************************************/
void set_parameter(void)
{
    /*最小值测试*/
    //	 out_fqc= 10;
    // 	 out_volt= 60;
    //	/*skip-read valout_volte = 23040 * output frquency / sample frequency  1s内有23040 * output frquency个点，采样其中的10000个点*/
    //	sin_cut = (u16)(((u32)out_fqc*23040) / 10000);		//23= 0x0017
    //	/* voltage command value * carrier /4 */
    //	volt_cmd = (((u32)out_volt*(C_CYCLE/2))/380);     //568=0x0238
    //
    /*最大值测试*/
    //	 out_fqc= 50;
    // 	 out_volt= 380;
    //	/*skip-read valout_volte = 23040 * output frquency / sample frequency  1s内有23040 * output frquency个点，采样其中的10000个点*/
    //	sin_cut = (u16)(((u32)out_fqc*23040) / 10000);		//115= 0x0073
    //	/* voltage command value * carrier /4 */
    //	volt_cmd = (((u32)out_volt*(C_CYCLE/2))/380);     // 3600= 0x0E10

    /*skip-read valout_volte = 23040 * output frquency / sample frequency  1s内有23040 * output frquency个点，采样其中的10000个点*/
    sin_cut = (u16)(((u32)out_fqc * 23040) / 10000);
    /* voltage command value * carrier /4 */
    volt_cmd = (((u32)out_volt * (C_CYCLE / 2)) / 380);
}
/*************************************************
FunctionName: void Key_Value_Deal(unsigned int key_value)
Description:  按键处理函数
Input:        键值
Output:       无
Return:       无
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
        GUI_DISP_HZ(1, 2, "当前频率：   Hz ");
        GUI_DISP_88_NUN_NEW(6, 2, out_fqc);
        GUI_DISP_HZ(1, 3, "理论电压：   V  ");
        GUI_DISP_888_NUN(6, 3, out_volt, 0);
        set_parameter();//刷新一下采样间隔和幅值
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
        GUI_DISP_HZ(1, 2, "当前频率：   Hz ");
        GUI_DISP_88_NUN_NEW(6, 2, out_fqc);
        GUI_DISP_HZ(1, 3, "理论电压：   V  ");
        GUI_DISP_888_NUN(6, 3, out_volt, 0);
        set_parameter();//刷新一下采样间隔和幅值
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
Description:  初始化继电器端口并开启延时继电器，短路延时电阻
Input:        无
Output:       无
Return:       无
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
Description:  主函数
Input:        无
Output:       无
Return:       成功运行返回1，失败返回0
*************************************************/
int main(void)
{
    Delay_JDQ_Init();    //继电器断开，保护电阻串入主回路
	  WorkLED_GPIO_Config();//工作指示灯初始化，单片机引脚默认为高，拉低指示灯亮说明单片机工作
	  SysTick_Init();//滴答定时器初始化，中断周期1ms，主要用于延时计时	  
	
	  Delay(500);//等待12864供电稳定
	  GUI_LCD_Init();   //12864显示屏初始化	
    GUI_LCD_CLR(0x00);//清屏
	  GUI_DISP_HZ(2, 1, "泰普克变频器");
		
	  /*工作灯闪烁两下后接通延时继电器*/
		WorkLED(ON);
	  GUI_DISP_HZ(2, 2, "初始化中3");
		Delay(500);//延时500ms共延时2s
  	WorkLED(OFF);
	  GUI_DISP_HZ(2, 2, "初始化中2");
	  Delay(500);
	  WorkLED(ON);
	  GUI_DISP_HZ(2, 2, "初始化中1");
		Delay(500);
		WorkLED(OFF);
    GUI_DISP_HZ(2, 2, "初始化完毕！");	
	  Delay(500);
	  WorkLED(ON);
	  GPIO_SetBits(GPIOC, GPIO_Pin_12);	//继电器闭合，短路保护电阻，开始工作
	  

   
    Key_GPIO_Config();          //按键初始化
    SPWM_Config();       //SPWM波形输出设置
    NVIC_Configuration();//中断优先级设置
		
    out_volt = 7 * out_fqc + 30;//对V/F曲线进行了补偿
    GUI_DISP_HZ(1, 2, "当前频率：   Hz ");
    GUI_DISP_88_NUN_NEW(6, 2, out_fqc);
    GUI_DISP_HZ(1, 3, "理论电压：   V  ");
    GUI_DISP_888_NUN(6, 3, out_volt, 0);
    GUI_DISP_HZ(2, 1, "泰普克变频器");

    while(1)
    {
        Key_Value_Deal(Key_Value_Get());//按键处理函数
		}

}



/************************END OF FILE**************************/
