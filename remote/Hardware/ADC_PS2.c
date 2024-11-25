
#include "SYS.h"


vu16 ADC_ConvertedValue[4];  //遥控通道ADC值
vu16 ADC_Calibrator[4];      //遥控通道ADC校准值
volatile bool ADC_CALIBRATOR_OK;   //遥控通道ADC校准标志
vu16 ADC_Value[4];		//用来保存ADC各通道转换完成后的数据
uint8_t dataPID = 0;		    // 数据包识别PID


void ADC_PS2_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;   // 定义结构体
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;   // 模拟输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//配置ADC采样参考时钟的预分频值
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);	

	// ADC参数配置
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;   // 独立模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;   // 非扫描模式	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;   // 关闭连续转换
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T4_CC4;   //TIM4的通道4的CCR触发采样
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;   // 右对齐	
	ADC_InitStructure.ADC_NbrOfChannel = 4;   // 1个转换在规则序列中 也就是只转换规则序列1 
	ADC_Init(ADC1, &ADC_InitStructure);   // ADC初始化
	
	
	//配置DMA1通道1，将ADC采样转换得到的数据传输到内存数组中
	DMA_InitStructure.DMA_BufferSize = 4;								//每次传输的数据的个数，传输完时触发中断
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;					//传输方向为：外设->内存
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;								//失能内存到内存的传输方式
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_ConvertedValue;				//数据保存到内存中数组的首地址（这里因为ADC_ConvertedValue是数组名，所以不用加&）
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;			//以16位为单位进行数据的传输
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						//内存地址自增（这里地址是每次增加1，因为是以半字（16位）作为单位传输的）
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;								//循环传输的方式，这里必须为循环传输方式，否则会导致DMA只能传输一次
	DMA_InitStructure.DMA_PeripheralBaseAddr = ((u32)&ADC1->DR);				//&ADC1->DR
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//以半字为单位进行数据的传输
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			//外设地址固定
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;						//DMA通道1的优先级设置为中级，（这个优先级是当同一个DMA的不同通道同时有传输数据的要求时，优先级高的先进行传输）
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);

	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);								//打开DMA通道1数据传输完成中断
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);											//打开NVIC中对应的DMA通道1的中断通道
	DMA_Cmd(DMA1_Channel1,ENABLE);

	//开启DMA1的通道1
	ADC_Cmd(ADC1, ENABLE);   // 开启AD转换器
	ADC_DMACmd(ADC1, ENABLE);
	
	//配置ADC的规则通道的采样顺序和采样时间
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_239Cycles5);	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_2,3,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_3,4,ADC_SampleTime_239Cycles5);
	
	// ADC校准
	ADC_ResetCalibration(ADC1);   // 重置指定的ADC的校准寄存器
	while(ADC_GetResetCalibrationStatus(ADC1));   // 获取ADC重置校准寄存器的状态
	ADC_StartCalibration(ADC1);   // 开始指定ADC的校准状态
	while(ADC_GetCalibrationStatus(ADC1));   // 获取指定ADC的校准程序

	//使能外部触发ADC采样
	ADC_ExternalTrigConvCmd(ADC1,ENABLE);

}
u8 packetData[12];

uint16_t Count_DMA=0;

//DMA1通道1中断通道处理函数，触发此中断时，100ms一次
void DMA1_Channel1_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC1)==SET)
	{
		CAL();	
		if(ADC_CALIBRATOR_OK)
		{
			ADC_Calibration();
		}
		Fly_UnLock();//按键处理
		PackData();
		SI24R1_SendPacket(packetData);//将SI24R1配置成发射模式
		DMA_ClearITPendingBit(DMA1_IT_TC1);		//清除DMA1通道1传输完成中断
	}
}



	
void ADC_Calibration(void)
{
	static u8 cnt=0;
	static vs32 temp1=0,temp2=0,temp3=0,temp4=0;
	if(ADC_ConvertedValue[2]>1900&&ADC_ConvertedValue[2]<2196&&ADC_ConvertedValue[3]>1900&&ADC_ConvertedValue[3]<2196)
	{
		if(0==cnt)
		{
			temp1 = 0;
			temp2 = 0;
			temp3 = 0;
			temp4 = 0;
			cnt = 0;
		}
		temp1 += (1000 + ADC_ConvertedValue[0]*1000/4096);
		temp2 += (1000 + ADC_ConvertedValue[1]*500/4096);
		temp3 += (1000 + ADC_ConvertedValue[2]*1000/4096);
		temp4 += (1000 + ADC_ConvertedValue[3]*1000/4096);
		cnt++;
		if(cnt >= 60)
		{
			ADC_Calibrator[0] = temp1/cnt;
			ADC_Calibrator[1] = temp2/cnt;
			ADC_Calibrator[2] = temp3/cnt;
			ADC_Calibrator[3] = temp4/cnt;
			ADC_CALIBRATOR_OK = 0;
			cnt = 0;

		}
	}
}


void CAL(void)
{
	if ((4096 - ADC_ConvertedValue[3]) < 6 && (4096 - ADC_ConvertedValue[1]) < 6) 
	{
		ADC_CALIBRATOR_OK = 1;
		u8g2_ClearBuffer(&u8g2);
		
	}
}






vu16 ADC_ValueLimit(vu16 value, vu16 L, vu16 R, vu16 min, vu16 max)
{
	vu16 result = 0;
	if (value > L && value < R)
		value = 1500;
	if (value < L)
		value += 1500 - L;
	if (value > R)
		value -= 1500 - L;

	value = (value <= min) ? min : value; // 防止越界
	value = (value >= max) ? max : value; // 防止越界

	result = value;
	return result;
}
/*
左Y  ADC_ConvertedValue[1]   往下最大
左X  ADC_ConvertedValue[0]   往左最大
右Y  ADC_ConvertedValue[3]   往左最大
右X  ADC_ConvertedValue[2]   往下最大
*/


void PackData(void)
{


	ADC_Value[0] = 1500 + ((1000 + ADC_ConvertedValue[0] * 1000 / 4096) - ADC_Calibrator[0]);
	ADC_Value[0] = ADC_ValueLimit(ADC_Value[0], 1470, 1530, 1100, 1900);

	ADC_Value[1] = ((4096 - ADC_ConvertedValue[1]) * 1000 / 4096); // 油门0~1000;

	ADC_Value[2] = 1500 + ((1000 + (ADC_ConvertedValue[2]) * 1000 / 4096) - ADC_Calibrator[2]);
	ADC_Value[2] = ADC_ValueLimit(ADC_Value[2], 1470, 1530, 1100, 1900);

	ADC_Value[3] = 1500 + ((1000 + (ADC_ConvertedValue[3]) * 1000 / 4096) - ADC_Calibrator[3]);
	ADC_Value[3] = ADC_ValueLimit(ADC_Value[3], 1470, 1530, 1100, 1900);
	
	
	if(dataPID > 200)
	{
		dataPID=0;
	}else
	{
		dataPID++;
	}
	
	packetData[0] =0xA8;	
	packetData[1] =*(((uint8_t *)ADC_Value)+0);	//偏航舵向低位 YAW
	packetData[2] =*(((uint8_t *)ADC_Value)+1);	//偏航舵向高位
	packetData[3] =*(((uint8_t *)ADC_Value)+2); //油门低位		
	packetData[4] = *(((uint8_t *)ADC_Value)+3);//油门高位		
	packetData[5] = *(((uint8_t *)ADC_Value)+4);//横滚舵向低位 ROLL			
	packetData[6] = *(((uint8_t *)ADC_Value)+5);//横滚舵向高位				
	packetData[7] = *(((uint8_t *)ADC_Value)+6);//俯仰舵向低位 PITCH			
	packetData[8] = *(((uint8_t *)ADC_Value)+7);//俯仰舵向高位 		
	packetData[9] = FLYUNLOCK;//解锁按键		
	packetData[10] = dataPID;		
	packetData[11] = 0xB8;	//校验码
}




// 接收飞机发送过来的数值
void ReceiveDataAnalysis(void)
{
	if (RxBuf[0] == 0xFF)
	{
		FLY.Lock = RxBuf[1];
		FLY.Thr = ((RxBuf[2] << 8) | RxBuf[3]);
		FLY.Yaw = ((s16)(RxBuf[4] << 8) | RxBuf[5]) / 100;
		FLY.Pit = ((s16)(RxBuf[6] << 8) | RxBuf[7]) / 100;
		FLY.Rol = ((s16)(RxBuf[8] << 8) | RxBuf[9]) / 100;
		FLY.BattV = (float)((((s16)(RxBuf[12] << 8) | RxBuf[13])) / 100);
	
	}
}