#include "adc.h"

#define ADC1_DR_Address    ((u32)0x40012400+0x4c)

__IO uint16_t ADC_Value_Tab[30];


/*************************************************
FunctionName: static void ADC1_GPIO_Config(void)
Description:  ʹ��ADC1��DMA1��ʱ�ӣ���ʼ��ADC�˿�
Input:        ��
Output:       ��
Return:       ��
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
    GPIO_Init(GPIOC, &GPIO_InitStructure);				// PC0,PC1,PC2,����ʱ������������

}
/*************************************************
FunctionName: static void ADC1_Mode_Config(void)
Description:  ����ADC1�Ĺ���ģʽΪMDAģʽ
Input:        ��
Output:       ��
Return:       ��
*************************************************/
static void ADC1_Mode_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    /* DMA channel1 configuration */
    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	    //ADC��ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_Value_Tab;//�ڴ��ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;              //���䷽�� ���衪>�ڴ�
    DMA_InitStructure.DMA_BufferSize = 30;                           //DMAͨ�������������
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ�̶�
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;        //�ڴ��ַ�̶�
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//���� 16bit
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//���� 16bit
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		//���ô���ģʽѭ������
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;//����DMA�����ȼ���
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;      //����DMA�����ڴ��еı����������
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    /* Enable DMA channel1 */
    DMA_Cmd(DMA1_Channel1, ENABLE);

    /* ADC1 configuration */
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//����ADCģʽ
    ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 	 //����ɨ��ģʽ��ɨ��ģʽ���ڶ�ͨ���ɼ�
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//��������ת��ģʽ������ͣ�ؽ���ADCת��
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//��ʹ���ⲿ����ת��
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	//�ɼ������Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel = 3;	 	//Ҫת����ͨ����Ŀ3,ע��DMA_BufferSize=30,ÿ��ͨ����ת��10��
    ADC_Init(ADC1, &ADC_InitStructure);

    /*ADC1regular channels configuration [����ģʽͨ������]*/
    ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_55Cycles5);	/*����ADC1��ͨ��0Ϊ55.	5���������ڣ�����Ϊ1 */
    ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_55Cycles5);	/*����ADC1��ͨ��1Ϊ55.	5���������ڣ�����Ϊ2 */
    ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_55Cycles5);	/*����ADC1��ͨ��2Ϊ55.	5���������ڣ�����Ϊ3 */

    /*����ADCʱ�ӣ�ΪPCLK2��8��Ƶ����9Hz*/
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);


    /* Enable ADC1 DMA */
    ADC_DMACmd(ADC1, ENABLE);

    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);

    /*��λУ׼�Ĵ��� */
    ADC_ResetCalibration(ADC1);
    /*�ȴ�У׼�Ĵ�����λ��� */
    while(ADC_GetResetCalibrationStatus(ADC1));

    /* ADCУ׼ */
    ADC_StartCalibration(ADC1);
    /* �ȴ�У׼���*/
    while(ADC_GetCalibrationStatus(ADC1));

    /* ����û�в����ⲿ����������ʹ���������ADCת�� */
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/*************************************************
FunctionName:void ADC1_Init(void)
Description: ADC1��ʼ��
Input:        ��
Output:       ��
Return:       ��
*************************************************/
void ADC1_Init(void)
{
    ADC1_GPIO_Config();
    ADC1_Mode_Config();
}

/*************************************************
FunctionName:unsigned int ADC1_Get_Value(unsigned char Chinel_num)
Description: ��ȡADC1ĳ��ͨ����ת��ֵ
Input:        ͨ��ֵ
Output:       ��
Return:       ��
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

    for(i = 0; i < 10; i++)	  	   	  				// �����ݽ���������
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

