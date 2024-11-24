#include "sys.h"
#define N 20 // 滤波缓存数组大小


_st_Mpu MPU6050;   //MPU6050原始数据
_st_AngE Angle;    //当前角度姿态值
_st_ALL_flag ALL_flag;

BATT_TYPE BAT =
{
    .BattAdc = 0,       // 电池电压采集ADC值
    .BattRealV = 3.31f,  // 实际测量的飞机供电电压 (注意此电压必须亲测否则测量的电压不准)
    .BattMeasureV = 0,  // 程序测量的实际电池电压
    .BattAlarmV = 3.2f, // 电池低电压报警瞬时值 (这个值需要根据机身不同重量实测，实测380mh是2.8v)
    .BattFullV = 4.2f,  // 电池充满电值 4.2V
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
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设置中断优先级分组  
  
	 NVIC_InitTypeDef NVIC_InitStruct;
	
	
//	NVIC_InitStruct.NVIC_IRQChannel=TIM4_IRQn;   //TIM4中断通道
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;   //抢占优先级0
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;   //子优先级1
//	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;   //使能TIM4中断通道
//	NVIC_Init(&NVIC_InitStruct);   //中断优先级初始化函数
	
	NVIC_InitStruct.NVIC_IRQChannel=USART1_IRQn;  //USART1中断通道
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel=EXTI2_IRQn;   //配置外部中断通道
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;   //设置抢占优先级为0
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;   //设置子优先级为1
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;   //使能外部中断通道
	NVIC_Init(&NVIC_InitStruct);   //中断优先级初始化函数

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

    ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;                  // 独立模式
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;              // 数据右对齐
    ADC_InitStruct.ADC_NbrOfChannel = 1;                             // 1个数据通道
    ADC_InitStruct.ADC_ScanConvMode = DISABLE;                       // 扫描转换模式失能
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 外部触发失能
    ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;                 // 连续转换失能
    ADC_Init(ADC1, &ADC_InitStruct);

    ADC_Cmd(ADC1, ENABLE); 
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5); // 规则组转换通道

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
    BAT.BattMeasureV = 2 * V; // 根据原理电阻分压，可知 电池实际电压 = ADC侧量电压 * 2
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
        return; // 数组填不满不计算
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





