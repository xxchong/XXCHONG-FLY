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



#define SYSTEM_SUPPORT_OS 1  //����ϵͳ��ʱ����define
#define MOTPR_MAX_PWM 700   //���ı�������PWM�ٶ�
//��ص�ѹ�������ݽṹ
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


//���ݲ�ֺ궨�壬�ڷ��ʹ���1�ֽڵ���������ʱ������int16��float�ȣ���Ҫ�����ݲ�ֳɵ����ֽڽ��з���
#define Byte0(data)       ( *( (char *)(&data)	  ) )
#define Byte1(data)       ( *( (char *)(&data) + 1) )
#define Byte2(data)       ( *( (char *)(&data) + 2) )
#define Byte3(data)       ( *( (char *)(&data) + 3) )


extern RC_TYPE RC;
extern uint8_t SI24R1_Controlflag ;
extern bool FLY_UnLock;
extern uint8_t DataID;	//���ݰ�ID

extern u8 FLYDataRx_OK;   

extern PidObject pidRateX; //�ڻ�PID����
extern PidObject pidRateY;
extern PidObject pidRateZ;

extern PidObject pidPitch; //�⻷PID����
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
