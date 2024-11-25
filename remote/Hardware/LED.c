#include "SYS.h"

void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = LED1|LED2|LED3|LED4|LED5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, LED1|LED2|LED3|LED4|LED5);
}

void LED_On(u16 LEDx)
{
	GPIO_ResetBits(GPIOB,LEDx);
}

void LED_Off(u16 LEDx)
{
	GPIO_SetBits(GPIOB,LEDx);
}

void LED_Toggle(u16 LEDx)
{
	GPIOB->ODR ^= LEDx;	
}

