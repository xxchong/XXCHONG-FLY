#ifndef __REMOTE_H
#define __REMOTE_H


#define SUCCESS 0
#undef FAILED  //ȡ��֮ǰ����� FAILED �ꡣ
	#define FAILED  1

#include <stdbool.h>
#include <stdint.h>


typedef struct tagRemoteConnect
{
	uint16_t	ConnectTimeCnt;		//����ʱ���ʱ��ÿ3ms�Լӣ�	
	uint8_t		ConnectState;		//1-���ӣ�0-δ����
	uint8_t		RemoteSignalLost;	//�źŶ�ʧ
	uint16_t	RemSigLostTimeCnt;	//�źŶ�ʧʱ���ʱ

}RemoteConnect_Str;


//ң���������ݽṹ 
typedef struct
{
	int16_t ROLL;
	int16_t PITCH;
	int16_t THROTTLE;
	int16_t YAW;
}RC_TYPE;
void Remote_Data_ReceiveAnalysis(void);





#endif


