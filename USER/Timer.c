#include "timer.h"
#include "math.h"

/***********************************************************************************
FunctionName: void SPWM_Config( void )
Description:  SPWM�������ĳ�ʼ������������ʱ�����ã��˿����ã���ʱ������
Input:        ��
Output:       ��
Return:       ��
***********************************************************************************/
void SPWM_Config( void )
{
    TIM_RCC_Config();
    TIM_GPIO_Config();
    TIM_Config();
}

/***********************************************************************************
FunctionName: void TIM_RCC_Config(void)
Description:  �Ͷ˿ڣ���ʱ���йص�ʱ������
Input:        ��
Output:       ��
Return:       ��
***********************************************************************************/
void TIM_RCC_Config(void)
{
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3, ENABLE );
}

/***********************************************************************************
FunctionName: void TIM_GPIO_Config(void)
Description:  �˿�����
Input:        ��
Output:       ��
Return:       ��
***********************************************************************************/
void TIM_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1,ENABLE);

    /* GPIOE Configuration: Channel 1, as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /*GPIOB Configuration:   as alternate function push-pull*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/***********************************************************************************
FunctionName: void TIM_GPIO_Config(void)
Description:  ��ʱ������
Input:        ��
Output:       ��
Return:       ��
***********************************************************************************/
void TIM_Config(void)
{
    /* Time Base configuration */
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    /********************TIM1����,���ڲ����ز�*********************************/
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_BDTRInitTypeDef TIM_BDTRInitStructure;

    TIM_TimeBaseStructure.TIM_Prescaler = 0;    //����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ72MHz
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;////����ʱ�ӷ�Ƶϵ��������Ƶ
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1 ;
    TIM_TimeBaseStructure.TIM_Period = C_CYCLE;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    /* Channel 1,  Configuration in PWM mode */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity =  TIM_OCPolarity_High;   //�ߵ�ƽ��Ч
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;  //�ߵ�ƽ��Ч
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;  //������Ϊ�ߵ�ƽ
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset; //������Ϊ�ߵ�ƽ
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);

    //��������
    TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
    TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
    TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1;
    TIM_BDTRInitStructure.TIM_DeadTime = 0xAC; //0xED=10us   0xAC=3us//�������������С0-0xff
    TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable;
    TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_Low;//ɲ������͵�ƽ��Ч
    TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
    TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);
		
    //����CCR1�Ĵ�����Ӱ�ӼĴ�����ֱ�����������¼��Ÿ������ã�
    //TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Disable);
    //TIM1_OC2ģ�����ã�����2ͨ��ռ�ձȣ�
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    //����CCR2�Ĵ�����Ӱ�ӼĴ�����ֱ�����������¼��Ÿ������ã� 
    //TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);

    //TIM1_OC3ģ�����ã�����3ͨ��ռ�ձȣ�
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);
    //����CCR2�Ĵ�����Ӱ�ӼĴ�����ֱ�����������¼��Ÿ������ã�
    //TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);

    /* TIM1 interrupt enable */
    TIM_ITConfig( TIM1, TIM_IT_Update, ENABLE );
    /* TIM1 counter enable */
    TIM_Cmd(TIM1, ENABLE);
    /* TIM1 Main Output Enable */
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
    /********************TIM2����,ÿ100us���ж�һ�Σ��ı�������ռ�ձȼ���ֵ**********************************/
    TIM_TimeBaseStructure.TIM_Prescaler = 19; //��Ƶϵ��Ϊ10
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;
    TIM_TimeBaseStructure.TIM_Period = 360;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;

    TIM_TimeBaseInit( TIM2, &TIM_TimeBaseStructure );
    TIM_ITConfig( TIM2, TIM_IT_Update, ENABLE );
    TIM_Cmd( TIM2, ENABLE );
	  /********************TIM3����,ÿ100ms���ж�һ��****************************************/
//		TIM_TimeBaseStructure.TIM_Prescaler= 999;
//		TIM_TimeBaseStructure.TIM_CounterMode= TIM_CounterMode_Down;
//		TIM_TimeBaseStructure.TIM_Period= 3600;
//		TIM_TimeBaseStructure.TIM_ClockDivision= 0;

//		TIM_TimeBaseInit( TIM3, &TIM_TimeBaseStructure );
//   	//TIM_SelectOutputTrigger( TIM3, TIM_TRGOSource_Update );// ADC Trigger  
//		TIM_ITConfig( TIM3, TIM_IT_Update, ENABLE );
//		TIM_Cmd( TIM3, DISABLE);
}



