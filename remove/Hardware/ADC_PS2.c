
#include "SYS.h"


vu16 ADC_ConvertedValue[4];  //ң��ͨ��ADCֵ
vu16 ADC_Calibrator[4];      //ң��ͨ��ADCУ׼ֵ
volatile bool ADC_CALIBRATOR_OK;   //ң��ͨ��ADCУ׼��־
vu16 ADC_Value[4];		//��������ADC��ͨ��ת����ɺ������
uint8_t dataPID = 0;		    // ���ݰ�ʶ��PID


void ADC_PS2_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;   // ����ṹ��
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;   // ģ������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//����ADC�����ο�ʱ�ӵ�Ԥ��Ƶֵ
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);	

	// ADC��������
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;   // ����ģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;   // ��ɨ��ģʽ	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;   // �ر�����ת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T4_CC4;   //TIM4��ͨ��4��CCR��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;   // �Ҷ���	
	ADC_InitStructure.ADC_NbrOfChannel = 4;   // 1��ת���ڹ��������� Ҳ����ֻת����������1 
	ADC_Init(ADC1, &ADC_InitStructure);   // ADC��ʼ��
	
	
	//����DMA1ͨ��1����ADC����ת���õ������ݴ��䵽�ڴ�������
	DMA_InitStructure.DMA_BufferSize = 4;								//ÿ�δ�������ݵĸ�����������ʱ�����ж�
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;					//���䷽��Ϊ������->�ڴ�
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;								//ʧ���ڴ浽�ڴ�Ĵ��䷽ʽ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_ConvertedValue;				//���ݱ��浽�ڴ���������׵�ַ��������ΪADC_ConvertedValue�������������Բ��ü�&��
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;			//��16λΪ��λ�������ݵĴ���
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						//�ڴ��ַ�����������ַ��ÿ������1����Ϊ���԰��֣�16λ����Ϊ��λ����ģ�
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;								//ѭ������ķ�ʽ���������Ϊѭ�����䷽ʽ������ᵼ��DMAֻ�ܴ���һ��
	DMA_InitStructure.DMA_PeripheralBaseAddr = ((u32)&ADC1->DR);				//&ADC1->DR
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//�԰���Ϊ��λ�������ݵĴ���
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			//�����ַ�̶�
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;						//DMAͨ��1�����ȼ�����Ϊ�м�����������ȼ��ǵ�ͬһ��DMA�Ĳ�ͬͨ��ͬʱ�д������ݵ�Ҫ��ʱ�����ȼ��ߵ��Ƚ��д��䣩
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);

	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);								//��DMAͨ��1���ݴ�������ж�
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);											//��NVIC�ж�Ӧ��DMAͨ��1���ж�ͨ��
	DMA_Cmd(DMA1_Channel1,ENABLE);

	//����DMA1��ͨ��1
	ADC_Cmd(ADC1, ENABLE);   // ����ADת����
	ADC_DMACmd(ADC1, ENABLE);
	
	//����ADC�Ĺ���ͨ���Ĳ���˳��Ͳ���ʱ��
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_239Cycles5);	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_2,3,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_3,4,ADC_SampleTime_239Cycles5);
	
	// ADCУ׼
	ADC_ResetCalibration(ADC1);   // ����ָ����ADC��У׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC1));   // ��ȡADC����У׼�Ĵ�����״̬
	ADC_StartCalibration(ADC1);   // ��ʼָ��ADC��У׼״̬
	while(ADC_GetCalibrationStatus(ADC1));   // ��ȡָ��ADC��У׼����

	//ʹ���ⲿ����ADC����
	ADC_ExternalTrigConvCmd(ADC1,ENABLE);

}
u8 packetData[12];

uint16_t Count_DMA=0;

//DMA1ͨ��1�ж�ͨ�����������������ж�ʱ��100msһ��
void DMA1_Channel1_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC1)==SET)
	{
		CAL();	
		if(ADC_CALIBRATOR_OK)
		{
			ADC_Calibration();
		}
		Fly_UnLock();//��������
		PackData();
		SI24R1_SendPacket(packetData);//��SI24R1���óɷ���ģʽ
		DMA_ClearITPendingBit(DMA1_IT_TC1);		//���DMA1ͨ��1��������ж�
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

	value = (value <= min) ? min : value; // ��ֹԽ��
	value = (value >= max) ? max : value; // ��ֹԽ��

	result = value;
	return result;
}
/*
��Y  ADC_ConvertedValue[1]   �������
��X  ADC_ConvertedValue[0]   �������
��Y  ADC_ConvertedValue[3]   �������
��X  ADC_ConvertedValue[2]   �������
*/


void PackData(void)
{


	ADC_Value[0] = 1500 + ((1000 + ADC_ConvertedValue[0] * 1000 / 4096) - ADC_Calibrator[0]);
	ADC_Value[0] = ADC_ValueLimit(ADC_Value[0], 1470, 1530, 1100, 1900);

	ADC_Value[1] = ((4096 - ADC_ConvertedValue[1]) * 1000 / 4096); // ����0~1000;

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
	packetData[1] =*(((uint8_t *)ADC_Value)+0);	//ƫ�������λ YAW
	packetData[2] =*(((uint8_t *)ADC_Value)+1);	//ƫ�������λ
	packetData[3] =*(((uint8_t *)ADC_Value)+2); //���ŵ�λ		
	packetData[4] = *(((uint8_t *)ADC_Value)+3);//���Ÿ�λ		
	packetData[5] = *(((uint8_t *)ADC_Value)+4);//��������λ ROLL			
	packetData[6] = *(((uint8_t *)ADC_Value)+5);//��������λ				
	packetData[7] = *(((uint8_t *)ADC_Value)+6);//���������λ PITCH			
	packetData[8] = *(((uint8_t *)ADC_Value)+7);//���������λ 		
	packetData[9] = FLYUNLOCK;//��������		
	packetData[10] = dataPID;		
	packetData[11] = 0xB8;	//У����
}




// ���շɻ����͹�������ֵ
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