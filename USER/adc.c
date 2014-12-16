#include "adc.h"

#define ADC1_DR_Address    ((u32)0x40012400+0x4c)

__IO uint16_t ADC_Value_Tab[30];


/*************************************************
FunctionName: static void ADC1_GPIO_Config(void)
Description:  使能ADC1和DMA1的时钟，初始化ADC端口
Input:        无
Output:       无
Return:       无
*************************************************/
static void ADC1_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable DMA clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    /* Enable ADC1 and GPIOC clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC, ENABLE);

    /* Configure PC0,PC1,PC2   as analog input */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);				// PC0,PC1,PC2,输入时不用设置速率

}
/*************************************************
FunctionName: static void ADC1_Mode_Config(void)
Description:  配置ADC1的工作模式为MDA模式
Input:        无
Output:       无
Return:       无
*************************************************/
static void ADC1_Mode_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    /* DMA channel1 configuration */
    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	    //ADC地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_Value_Tab;//内存地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;              //传输方向 外设—>内存
    DMA_InitStructure.DMA_BufferSize = 30;                           //DMA通道传输的数据量
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址固定
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;        //内存地址固定
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//半字 16bit
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//半字 16bit
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		//设置传输模式循环传输
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;//设置DMA的优先级别
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;      //设置DMA两个内存中的变量互相访问
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    /* Enable DMA channel1 */
    DMA_Cmd(DMA1_Channel1, ENABLE);

    /* ADC1 configuration */
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//独立ADC模式
    ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 	 //开启扫描模式，扫描模式用于多通道采集
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//开启连续转换模式，即不停地进行ADC转换
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//不使用外部触发转换
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	//采集数据右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 3;	 	//要转换的通道数目3,注意DMA_BufferSize=30,每个通道会转换10次
    ADC_Init(ADC1, &ADC_InitStructure);

    /*ADC1regular channels configuration [规则模式通道配置]*/
    ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_55Cycles5);	/*配置ADC1的通道0为55.	5个采样周期，序列为1 */
    ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_55Cycles5);	/*配置ADC1的通道1为55.	5个采样周期，序列为2 */
    ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_55Cycles5);	/*配置ADC1的通道2为55.	5个采样周期，序列为3 */

    /*配置ADC时钟，为PCLK2的8分频，即9Hz*/
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);


    /* Enable ADC1 DMA */
    ADC_DMACmd(ADC1, ENABLE);

    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);

    /*复位校准寄存器 */
    ADC_ResetCalibration(ADC1);
    /*等待校准寄存器复位完成 */
    while(ADC_GetResetCalibrationStatus(ADC1));

    /* ADC校准 */
    ADC_StartCalibration(ADC1);
    /* 等待校准完成*/
    while(ADC_GetCalibrationStatus(ADC1));

    /* 由于没有采用外部触发，所以使用软件触发ADC转换 */
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/*************************************************
FunctionName:void ADC1_Init(void)
Description: ADC1初始化
Input:        无
Output:       无
Return:       无
*************************************************/
void ADC1_Init(void)
{
    ADC1_GPIO_Config();
    ADC1_Mode_Config();
}

/*************************************************
FunctionName:unsigned int ADC1_Get_Value(unsigned char Chinel_num)
Description: 获取ADC1某个通道的转换值
Input:        通道值
Output:       无
Return:       无
*************************************************/
unsigned int ADC1_Get_Value(unsigned char Chinel_num)
{
    unsigned int ADC1_Channelx_Value[10];
    unsigned temp;
    unsigned int i, j;

    for(i = 0; i < 10; i++)
    {
        ADC1_Channelx_Value[i] = ADC_Value_Tab[i * 3 + Chinel_num - 10];
    }

    for(i = 0; i < 10; i++)	  	   	  				// 对数据进行排序处理
    {
        for(j = 0; j < 10 - i; j++)
        {
            if(ADC1_Channelx_Value[j] > ADC1_Channelx_Value[j + 1])
            {
                temp = ADC1_Channelx_Value[j];
                ADC1_Channelx_Value[j] = ADC1_Channelx_Value[j + 1];
                ADC1_Channelx_Value[j + 1] = temp;
            }
        }
    }

    temp = 0;
    for(i = 1; i < 9; i++)
    {
        temp += ADC1_Channelx_Value[i];
    }
    temp >>= 3;

    temp = temp * 330.0 / 4095;
    return temp;
}


/***************************END OF FILE****************************/

