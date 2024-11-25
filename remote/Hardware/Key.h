#ifndef __KEY__H
#define __KEY__H

#include "stdbool.h"

#define Button_GPIO 	GPIOB
#define Button_1 	GPIO_Pin_12  
#define Button_2 	GPIO_Pin_13
#define Button_3 	GPIO_Pin_14  
#define Button_4 	GPIO_Pin_15


typedef enum{
	Button_1_Single = 1,
	Button_1_Long,
	Button_2_Single,
	Button_2_Long,
	Button_3_Single,
	Button_3_Long,
	Button_4_Single,
	Button_4_Long,
}Button_Num;


//#define KEY_DOUBLE 1 /*使用单击双击长按功能*/
/*默认使用单击长按功能*/

#ifdef KEY_DOUBLE 
typedef struct{
	uint8_t Key_Sta;
	uint8_t Key_Judge;
	bool    Key_Flag_1;//单击
	bool    Key_Flag_2;//长按
	bool    Key_Flag_3;//双击
   uint8_t Key_Time;

   uint8_t key_interval;
   uint8_t key_twiceEn;

}KEY;
#else
typedef struct{
	uint8_t Key_Sta;
	uint8_t Key_Judge;
	bool    Key_Flag_1;//单击
	bool    Key_Flag_2;//长按
	bool    Key_Flag_3;//双击
	uint8_t Key_Time;
}KEY;


#endif
void Fly_UnLock(void);
void TIM2_IRQHandler(void);
uint8_t Key_Scan(void);
void KEY_Init(void);

#endif
