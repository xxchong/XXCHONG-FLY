#ifndef __SYS_H
#define __SYS_H

/* Device header */
#include "stm32f10x.h"                 
#include "stm32f10x_tim.h"

/* Standard Library */
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

/* FreeRTOS */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

/*System*/
#include "delay.h" 

/* Hardware */
#include "LED.h"
#include "Key.h"
#include "MyI2C.h"     
#include "OLED.h"
#include "Serial.h"
#include "timer.h"
#include "MySPI.h"
#include "si24r1.h"
#include "iic.h"

/*MPU6050*/
#include "mpu6050.h"
#include "imu.h"
#include "filter.h"
#include "kalman.h"
#include "myMath.h"

/*ANO_DT*/
#include "ANO_DT.h"

/*control*/
#include "PWM.h"
#include "PID.h"
#include "control.h"
#include "REMOTE.h"

#define SYSTEM_SUPPORT_OS 1   //系统支持OS
#define MOTPR_MAX_PWM 700    //电机最大PWM

#undef SUCCESS
	#define SUCCESS 0
#undef FAILED
	#define FAILED  1

//数据转换宏
#define Byte0(data)       ( *( (char *)(&data)	  ) )
#define Byte1(data)       ( *( (char *)(&data) + 1) )
#define Byte2(data)       ( *( (char *)(&data) + 2) )
#define Byte3(data)       ( *( (char *)(&data) + 3) )


//电池结构体
typedef struct BATT_TYPE
{
    float BattAdc;
    float BattRealV;
    float BattMeasureV;
    float BattAlarmV;
    float BattFullV;
} BATT_TYPE;



typedef volatile struct
{
	uint8_t unlock;
}_st_ALL_flag;





extern _st_Mpu MPU6050;
extern _st_AngE Angle;
extern _st_ALL_flag ALL_flag;



extern RC_TYPE RC;
extern uint8_t SI24R1_Controlflag ;
extern bool FLY_UnLock;
extern uint8_t DataID;	//数据ID

extern u8 FLYDataRx_OK;   

extern PidObject pidRateX; 
extern PidObject pidRateY;
extern PidObject pidRateZ;

extern PidObject pidPitch;
extern PidObject pidRoll;
extern PidObject pidYaw;


extern SemaphoreHandle_t xBinarySemaphore;
extern bool SendStatus;
extern uint8_t dataToSend[50];
extern BATT_TYPE BAT;


extern u8 RxBuf[RX_PLOAD_WIDTH];	
extern u8 TxBuf[TX_PLOAD_WIDTH];






void Battery_Init(void);
void NVIC_init(void);
uint16_t Get_BatteryAdc(uint8_t ch);
void BATT_GetVoltage(void);
void Aver_Filter(float data, float *filt_data, uint8_t n);

#endif
