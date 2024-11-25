#include "sys.h"           


void TIM3_PWM_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;  
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;   
	TIM_OCInitTypeDef TIM_OCInitStruct;  
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);   
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;  
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;   	  
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;  	
	GPIO_Init(GPIOA,&GPIO_InitStruct);   
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1; 
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;   	 
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;   	 
	GPIO_Init(GPIOB,&GPIO_InitStruct);  
	
	TIM_TimeBaseInitStruct.TIM_Period=1000-1;   	  
	TIM_TimeBaseInitStruct.TIM_Prescaler=100;   	//PWM������720HZ  
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;     
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);   		 
	
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;   
	TIM_OCInitStruct.TIM_Pulse=0;   							
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;   		
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;  

	TIM_OC1Init(TIM3,&TIM_OCInitStruct);   					
	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);  
	
/*	TIM_OC3PreloadConfig 函数用于配置TIM3的OC3输出的预装载功能。
	预装载功能通常用于在更新事件发生时平滑地改变输出比较寄存器的值，以避免在更新过程中产生不期望的输出变化。
*/	

	TIM_OC2Init(TIM3,&TIM_OCInitStruct);   						
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);   

	TIM_OC3Init(TIM3,&TIM_OCInitStruct);   					
	TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);  
	
	TIM_OC4Init(TIM3,&TIM_OCInitStruct);   						
	TIM_OC4PreloadConfig(TIM3,TIM_OCPreload_Enable);   
	
	TIM_Cmd(TIM3,ENABLE);   
}

/*************************************
 		 TIM3 CH1(PWM1) -> PA6
         TIM3 CH2(PWM2) -> PA7
         TIM3 CH3(PWM3) -> PB0
         TIM3 CH4(PWM4) -> PB1
***********************************/
//设置PWM
void Motor_Set_Pwm(uint16_t PWM1,uint16_t PWM2,uint16_t PWM3,uint16_t PWM4)
{
	TIM3->CCR1=PWM1;
	TIM3->CCR2=PWM2;
	TIM3->CCR3=PWM3;
	TIM3->CCR4=PWM4;
}

//限幅
void Motor_Pwm_Limit(uint16_t *PWM1,uint16_t *PWM2,uint16_t *PWM3,uint16_t *PWM4)
{
	
	if(*PWM1 > MOTPR_MAX_PWM)*PWM1=MOTPR_MAX_PWM;
	if(*PWM2 > MOTPR_MAX_PWM)*PWM2=MOTPR_MAX_PWM;
	if(*PWM3 > MOTPR_MAX_PWM)*PWM3=MOTPR_MAX_PWM;
	if(*PWM4 > MOTPR_MAX_PWM)*PWM4=MOTPR_MAX_PWM;
}

