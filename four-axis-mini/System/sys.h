#ifndef __SYS_H
#define __SYS_H


#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"     
#include "delay.h" 
#include "OLED.h"
#include "FreeRTOS.h"
#include "task.h"
#include "LED.h"
#include "Serial.h"
#include "timers.h"
#include "timer.h"
#include "Key.h"
#include "string.h"
#include "semphr.h"
#include <math.h>
#include "mpu6050.h"
#include "ANO_DT.h"
#include "imu.h"

#include "filter.h"
#include "kalman.h"
#include "myMath.h"

#include "stdbool.h"
#include "PWM.h"
#include "PID.h"
#include "control.h"
#include "MySPI.h"
#include "si24r1.h"
#include <stdio.h>
#include <stdarg.h>

#include <REMOTE.h>



#define SYSTEM_SUPPORT_OS 1  //操作系统延时函数define
#define MOTPR_MAX_PWM 700   //空心杯电机最大PWM速度
//电池电压管理数据结构
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



#undef SUCCESS
	#define SUCCESS 0
#undef FAILED
	#define FAILED  1

extern _st_Mpu MPU6050;

extern _st_AngE Angle;
extern _st_ALL_flag ALL_flag;


//数据拆分宏定义，在发送大于1字节的数据类型时，比如int16、float等，需要把数据拆分成单独字节进行发送
#define Byte0(data)       ( *( (char *)(&data)	  ) )
#define Byte1(data)       ( *( (char *)(&data) + 1) )
#define Byte2(data)       ( *( (char *)(&data) + 2) )
#define Byte3(data)       ( *( (char *)(&data) + 3) )


extern RC_TYPE RC;
extern uint8_t SI24R1_Controlflag ;
extern bool FLY_UnLock;
extern uint8_t DataID;	//数据包ID

extern u8 FLYDataRx_OK;   

extern PidObject pidRateX; //内环PID数据
extern PidObject pidRateY;
extern PidObject pidRateZ;

extern PidObject pidPitch; //外环PID数据
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
void Led_Init(void);
#endif
