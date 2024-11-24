#ifndef __REMOTE_H
#define __REMOTE_H


#define SUCCESS 0
#undef FAILED  //取消之前定义的 FAILED 宏。
	#define FAILED  1

#include <stdbool.h>
#include <stdint.h>


typedef struct tagRemoteConnect
{
	uint16_t	ConnectTimeCnt;		//连接时间计时，每3ms自加；	
	uint8_t		ConnectState;		//1-连接，0-未连接
	uint8_t		RemoteSignalLost;	//信号丢失
	uint16_t	RemSigLostTimeCnt;	//信号丢失时间计时

}RemoteConnect_Str;


//遥控器的数据结构 
typedef struct
{
	int16_t ROLL;
	int16_t PITCH;
	int16_t THROTTLE;
	int16_t YAW;
}RC_TYPE;
void Remote_Data_ReceiveAnalysis(void);





#endif


