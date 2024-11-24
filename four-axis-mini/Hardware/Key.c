#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


void KeyIntInit(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;//�����ʼ���ṹ��
	NVIC_InitTypeDef NVIC_InitStructure;//����ṹ��

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); /* ʹ��AFIO����ʱ�� */

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0); /* ��GPIO�����ж���ӳ������ */


	EXTI_InitStructure.EXTI_Line=EXTI_Line0; // �ж���
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;            // �ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // ˫���ش���

	EXTI_InitStructure.EXTI_LineCmd = ENABLE;

	EXTI_Init(&EXTI_InitStructure); // ��ʼ��	


	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;     //ʹ���ⲿ�ж����ڵ�ͨ��	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  // ��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         // �����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            // ʹ���ⲿ�ж�ͨ�� 	
	NVIC_Init(&NVIC_InitStructure); // ��ʼ�� 

}



