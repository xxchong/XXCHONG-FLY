#ifndef __REMOTE_H
#define __REMOTE_H
#include "stdbool.h"
//接收飞机发送过来的数值



typedef struct
{
	u16 Thr;   	/*油门*/
	s16 Yaw;	/*航向角*/
	s16 Rol;	/*横滚角*/
	s16 Pit;	/*俯仰角*/
	float BattV;	/*飞机电量*/
	bool Lock;	/*飞机锁*/
}FLY_TYPE;







#endif
