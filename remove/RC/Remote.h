#ifndef __REMOTE_H
#define __REMOTE_H
#include "stdbool.h"
//���շɻ����͹�������ֵ



typedef struct
{
	u16 Thr;   	/*����*/
	s16 Yaw;	/*�����*/
	s16 Rol;	/*�����*/
	s16 Pit;	/*������*/
	float BattV;	/*�ɻ�����*/
	bool Lock;	/*�ɻ���*/
}FLY_TYPE;







#endif
