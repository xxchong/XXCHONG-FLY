#ifndef __SYS_H
#define __SYS_H

/*STM32F103C8T6*/
#include "stm32f10x.h"    
#include "stm32f10x_tim.h"


/*FreeRTOS*/
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h" 
#include "queue.h"

/*c STANDARD*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

/*system*/
#include "Delay.h"
#include "MyRTC.h"

/*hardware*/
#include "LED.h"
#include "timer.h"
#include "Key.h"
#include "i2c.h"
#include "usart.h"
#include "MySPI.h"
#include "ADC_PS2.h"
#include "si24r1.h"

/*Remote*/
#include "Remote.h"

/*u8g2*/
#include "u8g2.h"
#include "u8x8.h"
#include "u8g2_font_MyChinese.h"
#include "DisplayOLED.h"

#define STOP  	 0x00
#define Forward  0x01
#define Backward 0x02
#define Left  	 0x03
#define Right  	 0x04

extern u8 FLYDataRx_OK;   

extern u8g2_t u8g2;
extern TaskHandle_t Task1_Handler;
extern TaskHandle_t Task2_Handler;
extern TaskHandle_t Task3_Handler;
extern SemaphoreHandle_t xBinarySemaphore; //二进制信号量
extern bool FLYUNLOCK;
extern vu16 ADC_ConvertedValue[4];  //遥控通道ADC值
extern u8 RxBuf[RX_PLOAD_WIDTH];	
extern u8 TxBuf[TX_PLOAD_WIDTH];	
extern uint8_t Page;
extern FLY_TYPE  FLY;
extern u8 FLYDataRx_OK;   
extern u8 TxBuf[TX_PLOAD_WIDTH];		
extern u8 RxBuf[RX_PLOAD_WIDTH];
extern volatile bool ADC_CALIBRATOR_OK;   //遥控通道ADC校准标志
////void EXTI15_10_IRQHandler(void);
extern vu16 ADC_Value[4];
void ShowStackSt(void);
void MyStr(char *STR,char *Return);
void OpenShow(void);
void Init_Selection(void);
void NVIC_init(void);
void NVIC_Config(void);

#endif