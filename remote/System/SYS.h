#ifndef __SYS_H
#define __SYS_H

#include "stm32f10x.h"                 
#include "Delay.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h" 
#include "queue.h"
#include "LED.h"
#include "timers.h"
#include "timer.h"
#include "Key.h"
#include "string.h"

#include <string.h>
#include <stdio.h>

#include "i2c.h"
#include "stdlib.h"

#include "MyRTC.h"
#include "u8g2.h"
#include "u8x8.h"
#include "u8g2_font_MyChinese.h"
#include <stdio.h>
#include <stdarg.h>
#include "usart.h"
#include "MySPI.h"
#include "ADC_PS2.h"
#include "si24r1.h"
#include "Remote.h"
#include "DisplayOLED.h"

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
