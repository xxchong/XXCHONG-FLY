#ifndef _IMU_H_
#define	_IMU_H_

#include "stdint.h"
typedef volatile struct 
{
  float q0;
  float q1;
  float q2;
  float q3;
} Quaternion;

typedef struct{
	int16_t accX;
	int16_t accY;
	int16_t accZ;
	int16_t gyroX;
	int16_t gyroY;
	int16_t gyroZ;
}_st_Mpu;

typedef struct{
	float roll;
	float pitch;
	float yaw;
}_st_AngE;


void GetAngle(const _st_Mpu *pMpu,_st_AngE *pAngE, float dt);


#endif
