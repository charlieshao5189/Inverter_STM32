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
vu16 out_fqc_targ = 1000;                  //输出频率，默认初始为10HZ
vu16 out_fqc_now;
vu16 out_volt;
vu16 volt_cmd = 0;          /* torque command value x carrier /4 */

volatile float sin_cut = 0;	      		 /* skip - read value for sine pointer */
vu16 sin_pt = 0;							/* angle */
vu16 sinpt_sum = 0;         /* sum of angle  */
//vu8 Moter_Start_Flag=0;

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
    pwm.ui  = (C_CYCLE /2) - pwm.ui;    // voltage command

    /* V phase output command */
    pwm.vi  = (s32)sin_tbl[sin_pt + 120] * (s32)(volt_cmd * 2) >> 16; ;
    pwm.vi =  (C_CYCLE /2) - pwm.vi;   // voltage command

    /* W phase output command */
    pwm.wi = (s32)sin_tbl[sin_pt + 240] * (s32)(volt_cmd * 2) >> 16;
    pwm.wi = (C_CYCLE /2) - pwm.wi;         // voltage command


    /* update buffer */
    pwm.uo = pwm.ui;
    pwm.vo = pwm.vi;
    pwm.wo = pwm.wi;

}


/***********************************************************************************
FunctionName: void set_parameter(void)
Description:  设置参数
Input:        u16 out_fqc:后两位为小数
Output:       sin_cut:采样间隔度数X64;volt_cmd:V/F值
Return:       无
***********************************************************************************/
void set_parameter(u16 out_fqc)
{
//    /*最小值测试*/
//		 out_fqc= 1000;
//		 out_volt= 10000;
//	   /*skip-read valout_volte = 23040 * output frquency / sample frequency  1s内有360 * output frquency个点，采样其中的10000个点*/
//    sin_cut= (float)out_fqc*23040/1000000;
//    /* voltage command value * carrier /4 */
//    volt_cmd = (((u32)out_volt * (C_CYCLE / 2)) / 38000);

//    /*最大值测试*/
//		 out_fqc= 5000;
//		 out_volt= 38000;
//	   /*skip-read valout_volte = 23040 * output frquency / sample frequency  1s内有360 * output frquency个点，采样其中的10000个点*/
//    sin_cut= (float)out_fqc*23040/1000000;
//    /* voltage command value * carrier /4 */
//    volt_cmd = (((u32)out_volt * (C_CYCLE / 2)) / 38000);
		out_volt = 7 * out_fqc + 3000;
		GUI_DISP_HZ(1, 3, "电压：         V");
		GUI_DISP_DEC(4, 3, out_volt, 2, 0);
	
    /*skip-read valout_volte = 23040 * output frquency / sample frequency  1s内有360 * output frquency个点，采样其中的10000个点*/
    sin_cut= (float)out_fqc/1000*2304/100;//(float)out_fqc*23040/1000000;
    /* voltage command value * carrier /4 */
    volt_cmd = (((u32)out_volt * (C_CYCLE / 2)) / 38000);
}
/*************************************************
FunctionName: void Key_Value_Deal(unsigned int key_value)
Description:  按键处理函数
Input:        键值
Output:       无
Return:       无
*************************************************/
vu16 key_count;
vu8  key_status;
#define key_count_interval key_count/10-5
extern unsigned int key_value_buf;
void Key_Value_Deal(unsigned int key_value)
{
	  if((key_value_buf!=0)&&(key_status==1))//长按键处理
	  {
			 if(key_value_buf==2)
			 { 
				       if(out_fqc_targ+key_count_interval>=5000)
							{
							 out_fqc_targ=5000;
							}
					    else out_fqc_targ+=key_count_interval ;//key_count= 大于100的奇数，out_fqc每次增加一个奇数				
			 } 
			 else if(key_value_buf==3)
			 {
				      if(out_fqc_targ<=key_count_interval)
							{
							 out_fqc_targ=0;
							}
					    else out_fqc_targ-=key_count_interval;//key_count= 大于10的奇数，out_fqc每次增加一个奇数
			 } 
			   key_status=0;
			   GUI_DISP_HZ(1, 2,"频率：        Hz");
         GUI_DISP_DEC(4, 2, out_fqc_targ, 2, 0);
		}
    switch(key_value)
    {
    case  KEY_UP_Value:
    {
			  if(out_fqc_targ>=5000)
			  {  
				   out_fqc_targ=4999;
				}
			  out_fqc_targ+=1;
        GUI_DISP_HZ(1, 2,"频率：        Hz");
        GUI_DISP_DEC(4, 2, out_fqc_targ, 2, 0);
    };
    break; //
    case  KEY_DOWN_Value :
    {
			  if(out_fqc_targ<=1)
			  {  
				   out_fqc_targ=1;
				}
			  out_fqc_targ-=1;
        GUI_DISP_HZ(1, 2,"频率：        Hz");
        GUI_DISP_DEC(4, 2, out_fqc_targ, 2, 0);
    };
    break;
    case  KEY_ENTER_Value :out_fqc_targ=5000;
        break; //
    case  KEY_BACK_Value :out_fqc_targ= 1;
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
//    Delay_JDQ_Init();    //继电器断开，保护电阻串入主回路
//	  WorkLED_GPIO_Config();//工作指示灯初始化，单片机引脚默认为高，拉低指示灯亮说明单片机工作
	  SysTick_Init();//滴答定时器初始化，中断周期1ms，主要用于延时计时	  
	
	  Delay(500);//等待12864供电稳定
	  GUI_LCD_Init();   //12864显示屏初始化	
    GUI_LCD_CLR(0x00);//清屏
	  GUI_DISP_HZ(2, 1, "泰普克变频器");
		
	  /*工作灯闪烁两下后接通延时继电器*/
//		WorkLED(ON);
	  GUI_DISP_HZ(2, 2, "初始化中3");
		Delay(500);//延时500ms共延时2s
//  	WorkLED(OFF);
	  GUI_DISP_HZ(2, 2, "初始化中2");
	  Delay(500);
//	  WorkLED(ON);
	  GUI_DISP_HZ(2, 2, "初始化中1");
		Delay(500);
//		WorkLED(OFF);
    GUI_DISP_HZ(2, 2, "初始化完毕！");	
	  Delay(500);
//	  WorkLED(ON);
	  GPIO_SetBits(GPIOC, GPIO_Pin_12);	//继电器闭合，短路保护电阻，开始工作
	  

   
    Key_GPIO_Config();   //按键初始化
    SPWM_Config();       //SPWM波形输出设置
    NVIC_Configuration();//中断优先级设置
		
    out_volt = 7 * out_fqc_targ + 3000;//对V/F曲线进行了补偿
    GUI_DISP_HZ(1, 2,"频率：        Hz");
    GUI_DISP_DEC(4, 2, out_fqc_targ, 2, 0);
    GUI_DISP_HZ(1, 3,"电压：        V");
    GUI_DISP_DEC(4, 3, out_volt, 2, 0);
    GUI_DISP_HZ(2, 1,"泰普克变频器");
		
    while(1)
    {
        Key_Value_Deal(Key_Value_Get());//按键处理函数
			  set_parameter(out_fqc_now);
	 	}

}



/************************END OF FILE**************************/
