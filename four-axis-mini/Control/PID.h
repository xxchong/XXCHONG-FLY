#ifndef __PID_H
#define	__PID_H

#include "stm32f10x.h"


typedef volatile struct
{
	float desired;     //< set point
	float offset;      //
	float prevError;    //< previous error
	float integ;        //< integral
	float kp;           //< proportional gain
	float ki;           //< integral gain
	float kd;           //< derivative gain
	float IntegLimitHigh;       //< integral limit
	float IntegLimitLow;
	float measured;
	float out;
	float OutLimitHigh;
	float OutLimitLow;
}PidObject;




extern void CascadePID(PidObject* pidRate,PidObject* pidAngE,const float dt); //串级PID
extern void pidRest(PidObject **pid,const uint8_t len); //pid数据归位
extern void pidUpdate(PidObject* pid,const float dt);  //PID

#endif