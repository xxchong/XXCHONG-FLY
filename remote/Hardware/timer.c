#include "timer.h"
#include "stm32f10x_tim.h"

//һ��ʱ�䴥������һ��ADCת��
void TIM_OCTigrConfig(void)
{
	TIM_TimeBaseInitTypeDef TIM_timeBaseStucture;
	TIM_OCInitTypeDef TIM_ocInitStructure;
	
	u16 period 	= 10000-1;										//��������ֵ100ms����һ��
	u16 pluse 	= 5000;											//����CRRֵ�������ֵ����������ã���Ӱ��ADC�������ڣ���Ҫ��֤����ȷ�ķ�Χ��
	
	//ʹ��TIM4��ʱ�ӣ�TIM4�ǹ���APB1�����ϵģ�ע��TIM4��ʱ����72M��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	TIM_timeBaseStucture.TIM_ClockDivision = TIM_CKD_DIV1;      	//��Ƶ���ӣ��������ʱ����ETRP�����˲����ṩʱ��
	TIM_timeBaseStucture.TIM_Prescaler = 720-1;                		//Ԥ��Ƶ����TIMx_CNT������ʱ��120
	TIM_timeBaseStucture.TIM_CounterMode = TIM_CounterMode_Up;		//���ϼ���
	TIM_timeBaseStucture.TIM_Period = period;						//�������ڣ���ARR��ֵ
	TIM_TimeBaseInit(TIM4,&TIM_timeBaseStucture);
	
	//����TIM4ͨ��4������Ƚ�
	TIM_ocInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				//PWM���ģʽΪPWM1
	TIM_ocInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//������Ч��ƽ�ļ���
	TIM_ocInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//ʹ��ͨ�����
	TIM_ocInitStructure.TIM_Pulse = pluse;							//����PWM��������ֵ����CRRֵ
	TIM_OC4Init(TIM4,&TIM_ocInitStructure);
	
	TIM_ARRPreloadConfig(TIM4,ENABLE);								//ʹ��TIM4�ļĴ���ARR��Ԥװ�ع��ܣ�DISABLEʱ����ʹ�ı�ARRֵʱ������Ч
	TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable);				//ʹ��TIM4ͨ��1��CCR��Ԥװ�ع��ܣ�DISABLEʱ����ʹ�ı�CRRֵʱ������Ч
	
	//ʹ��TIM4��ʱ��
	TIM_Cmd(TIM4,ENABLE);
}




