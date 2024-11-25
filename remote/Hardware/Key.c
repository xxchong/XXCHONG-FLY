
#include "SYS.h"

KEY Keys[4]={0,0,0,0};
bool FLYUNLOCK=0;



void KEY_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIOA_InitTypeDef;
	GPIO_InitTypeDef GPIOB_InitTypeDef;
	TIM_TimeBaseInitTypeDef TIM2_TimeBaseInitTypeDef;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB ,ENABLE);
	
	GPIOA_InitTypeDef.GPIO_Mode=GPIO_Mode_IPU;
	GPIOA_InitTypeDef.GPIO_Pin=Button_1 | Button_2 | Button_3 |Button_4;
	GPIOA_InitTypeDef.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIOA_InitTypeDef);

	TIM_InternalClockConfig(TIM2);
	TIM2_TimeBaseInitTypeDef.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM2_TimeBaseInitTypeDef.TIM_CounterMode=TIM_CounterMode_Up;
	TIM2_TimeBaseInitTypeDef.TIM_Period=10000-1;
	TIM2_TimeBaseInitTypeDef.TIM_Prescaler=72-1;
	TIM_TimeBaseInit(TIM2, &TIM2_TimeBaseInitTypeDef);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM2,ENABLE);
	
}


#ifdef KEY_DOUBLE
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  // 清楚中断标志位
		Keys[0].Key_Sta=GPIO_ReadInputDataBit(Button_GPIO,Button_1);
		Keys[1].Key_Sta=GPIO_ReadInputDataBit(Button_GPIO,Button_2);
		Keys[2].Key_Sta=GPIO_ReadInputDataBit(Button_GPIO,Button_3);
		Keys[3].Key_Sta=GPIO_ReadInputDataBit(Button_GPIO,Button_4);
		for(int i=0; i<4; i++)
		{
			switch(Keys[i].Key_Judge){
				case 0: 
					{   /*第一次按下按键*/
						if(Keys[i].Key_Sta == 0)
						{
							Keys[i].Key_Judge=1;
							Keys[i].Key_Time=0;
						}
					
					 }
				break;
					 
			    case 1:
					{   /*10ms后按键（消抖）*/
						if(Keys[i].Key_Sta == 0)
						{
							Keys[i].Key_Judge=2;
						}
						else {
							Keys[i].Key_Judge=0;
						
						}
					 }
				break;
					 
			   case 2:
				     {    /*等待松开*/
						 if(Keys[i].Key_Sta == 1 && Keys[i].Key_Time < 70)
						{
							
						  if(Keys[i].key_twiceEn==0)
                           {
                               Keys[i].key_twiceEn = 1;/*双击的第一次按下*/
                               Keys[i].key_interval = 0;
                           }
                           else
                           {
                               Keys[i].Key_Flag_3 = 1;
                               Keys[i].key_twiceEn = 0;
                           }
							Keys[i].Key_Judge=0;
							
						}else if(Keys[i].Key_Sta == 1 && Keys[i].Key_Time > 70)
						{
								Keys[i].Key_Flag_2=1;
								Keys[i].Key_Judge=0;

						}else
						{
							Keys[i].Key_Time++;
						}
					
					 }
				break;
			}
			if(Keys[i].key_twiceEn == 1)/*双击的第一次按下*/
            {
                Keys[i].key_interval ++;
                if(Keys[i].key_interval >= 20)
                {
                    Keys[i].key_twiceEn = 0;
                    Keys[i].Key_Flag_1=1;				
                }
            }
		}	
	}
}

#else
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  // 清楚中断标志位
		Keys[0].Key_Sta=GPIO_ReadInputDataBit(Button_GPIO,Button_1);
		Keys[1].Key_Sta=GPIO_ReadInputDataBit(Button_GPIO,Button_2);
		Keys[2].Key_Sta=GPIO_ReadInputDataBit(Button_GPIO,Button_3);
		Keys[3].Key_Sta=GPIO_ReadInputDataBit(Button_GPIO,Button_4);
	   
		for(int i=0; i<4; i++)
		{
			switch(Keys[i].Key_Judge){
				case 0: 
					{   /*第一次按下按键*/
						if(Keys[i].Key_Sta == 0)
						{
							Keys[i].Key_Judge=1;
							Keys[i].Key_Time=0;
						}
					 }
				break;
					 
			    case 1:
					{   /*10ms后按键（消抖）*/
						if(Keys[i].Key_Sta == 0)
						{
							Keys[i].Key_Judge=2;
						}
						else {
							Keys[i].Key_Judge=0;
						}
					 }
				break;
					 
			   case 2:
				     {    /*等待松开*/
						 if(Keys[i].Key_Sta == 1 && Keys[i].Key_Time < 70)
						{
							Keys[i].Key_Flag_1=1;
							Keys[i].Key_Judge=0;
							
						}else if(Keys[i].Key_Sta == 1 && Keys[i].Key_Time > 70)
						{
								Keys[i].Key_Flag_2=1;
								Keys[i].Key_Judge=0;
						}else
						{
							Keys[i].Key_Time++;
						}
					 }
				break;
			}
		}	
	}
}

#endif



/*飞机解锁*/
void Fly_UnLock(void)
{
	if( Keys[2].Key_Flag_1 == 1  )
	{
		if((4096 - ADC_ConvertedValue[1])  < 6)/*油门最低时方可解锁或上锁*/
		{
			FLYUNLOCK=!FLYUNLOCK;
		}	
		Keys[2].Key_Flag_1 = 0;
	
	}
	
	if( Keys[0].Key_Flag_1 == 1  )
	{
		u8g2_ClearBuffer(&u8g2);
		if(++Page == 2 )Page = 0;
		Keys[0].Key_Flag_1 = 0;
	
	}

}

uint8_t Key_Scan(void)
{
    uint8_t KeyNum = 0;
    uint8_t Button_Single[] = {Button_1_Single, Button_2_Single, Button_3_Single, Button_4_Single};
    uint8_t Button_Long[] = {Button_1_Long, Button_2_Long, Button_3_Long, Button_4_Long};

    for (int i = 0; i < 4; i++) {
        if (Keys[i].Key_Flag_1 == 1) {
            Keys[i].Key_Flag_1 = 0;
            KeyNum = Button_Single[i];
            break;
        }
        if (Keys[i].Key_Flag_2 == 1) {
            Keys[i].Key_Flag_2 = 0;
            KeyNum = Button_Long[i];
            break;
        }
    }

    return KeyNum;
}