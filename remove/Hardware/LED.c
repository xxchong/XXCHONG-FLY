#include "SYS.h"

void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //����SW������JTAG��PA15��PB3��PB4����
	GPIO_InitTypeDef  GPIO_InitStructure;		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
}

//����LED��
void LED_On(u16 LEDx)
{
	GPIO_ResetBits(GPIOB,LEDx);
}

void LED_Off(u16 LEDx)
{
	GPIO_SetBits(GPIOB,LEDx);
}

//����LED
void LED_Toggle(u16 LEDx)
{
	GPIOB->ODR ^= LEDx;	//��򣬿���ģʽ����ߵ͵�ƽ
}

