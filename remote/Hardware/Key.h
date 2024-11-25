#ifndef __KEY__H
#define __KEY__H

#include "stdbool.h"

#define Button_1 	GPIO_Pin_12  
#define Button_2 	GPIO_Pin_13
#define Button_3 	GPIO_Pin_14  
#define Button_4 	GPIO_Pin_15

#define Button_GPIO 	GPIOB


#define Button_1_Single 	 1
#define Button_1_Long 	   2
#define Button_2_Single 	 3
#define Button_2_Long 	   4
#define Button_3_Single 	 5
#define Button_3_Long 	   6
#define Button_4_Single 	 7
#define Button_4_Long 	   8



//#define KEY_DOUBLE 1 /*ʹ�õ���˫����������*/
/*Ĭ��ʹ�õ�����������*/

#ifdef KEY_DOUBLE 
typedef struct{
	uint8_t Key_Sta;
	uint8_t Key_Judge;
	bool    Key_Flag_1;//����
	bool    Key_Flag_2;//����
	bool    Key_Flag_3;//˫��
   uint8_t Key_Time;

   uint8_t key_interval;
   uint8_t key_twiceEn;

}KEY;
#else
typedef struct{
	uint8_t Key_Sta;
	uint8_t Key_Judge;
	bool    Key_Flag_1;//����
	bool    Key_Flag_2;//����
	bool    Key_Flag_3;//˫��
	uint8_t Key_Time;
}KEY;

#endif
void Fly_UnLock(void);
void TIM2_IRQHandler(void);
uint8_t Key_Scan(void);
void KEY_Init(void);

#endif
