#include "sys.h"
#define N 20 // �˲����������С


_st_Mpu MPU6050;   //MPU6050ԭʼ����
_st_AngE Angle;    //��ǰ�Ƕ���ֵ̬
_st_ALL_flag ALL_flag;

BATT_TYPE BAT =
{
    .BattAdc = 0,       // ��ص�ѹ�ɼ�ADCֵ
    .BattRealV = 3.31f,  // ʵ�ʲ����ķɻ������ѹ (ע��˵�ѹ�����ײ��������ĵ�ѹ��׼)
    .BattMeasureV = 0,  // ���������ʵ�ʵ�ص�ѹ
    .BattAlarmV = 3.2f, // ��ص͵�ѹ����˲ʱֵ (���ֵ��Ҫ���ݻ���ͬ����ʵ�⣬ʵ��380mh��2.8v)
    .BattFullV = 4.2f,  // ��س�����ֵ 4.2V
};

void Led_Init(void)
{
   	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_8);
}

void NVIC_init(void)
{
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // �����ж����ȼ�����  
  
	 NVIC_InitTypeDef NVIC_InitStruct;
	
	
//	NVIC_InitStruct.NVIC_IRQChannel=TIM4_IRQn;   //TIM4�ж�ͨ��
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;   //��ռ���ȼ�0
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;   //�����ȼ�1
//	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;   //ʹ��TIM4�ж�ͨ��
//	NVIC_Init(&NVIC_InitStruct);   //�ж����ȼ���ʼ������
	
	NVIC_InitStruct.NVIC_IRQChannel=USART1_IRQn;  //USART1�ж�ͨ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel=EXTI2_IRQn;   //�����ⲿ�ж�ͨ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;   //������ռ���ȼ�Ϊ0
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;   //���������ȼ�Ϊ1
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;   //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStruct);   //�ж����ȼ���ʼ������

}






void Battery_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    ADC_InitTypeDef ADC_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN; 
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;                  // ����ģʽ
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;              // �����Ҷ���
    ADC_InitStruct.ADC_NbrOfChannel = 1;                             // 1������ͨ��
    ADC_InitStruct.ADC_ScanConvMode = DISABLE;                       // ɨ��ת��ģʽʧ��
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // �ⲿ����ʧ��
    ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;                 // ����ת��ʧ��
    ADC_Init(ADC1, &ADC_InitStruct);

    ADC_Cmd(ADC1, ENABLE); 
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5); // ������ת��ͨ��

}

uint16_t Get_BatteryAdc(uint8_t ch)
{
    ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE); 
    while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));                                
    return ADC_GetConversionValue(ADC1); 
}

void BATT_GetVoltage(void)
{
    float V;
    Aver_Filter((float)Get_BatteryAdc(ADC_Channel_1), &BAT.BattAdc, 6); 
    if (BAT.BattAdc)
        V = BAT.BattAdc * BAT.BattRealV / 4095.0f;
    BAT.BattMeasureV = 2 * V; // ����ԭ������ѹ����֪ ���ʵ�ʵ�ѹ = ADC������ѹ * 2
}

void Aver_Filter(float data, float *filt_data, uint8_t n)
{
    static float buf[N];
    static uint8_t cnt = 0, flag = 1;
    float temp = 0;
    uint8_t i;
    buf[cnt] = data;
    cnt++;
    if (cnt < n && flag)
        return; // �������������
    else
        flag = 0;

    for (i = 0; i < n; i++)
    {
        temp += buf[i];
    }
    if (cnt >= n)
        cnt = 0;
    *filt_data = temp / n;
}





