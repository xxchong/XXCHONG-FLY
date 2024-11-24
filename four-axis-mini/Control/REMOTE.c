#include "sys.h"

RemoteConnect_Str RemoteConnect;
RC_TYPE RC;
uint8_t SI24R1_Controlflag = 1;
bool FLY_UnLock;
uint8_t DataID;	//���ݰ�ID


void Remote_Data_ReceiveAnalysis(void)
{
	SI24R1_Controlflag = 1;
	if(RxBuf[11]!=0xB8)	//��֤У�����Ƿ�Ϊ0xa5
		return;
	if(RxBuf[0] & 0xA8)
	{
		RC.YAW      = RxBuf[2]<<8|RxBuf[1];		
		RC.THROTTLE = RxBuf[4]<<8|RxBuf[3]; 	
		RC.ROLL     = RxBuf[6]<<8|RxBuf[5];	  	
		RC.PITCH    = RxBuf[8]<<8|RxBuf[7];	
		FLY_UnLock	= RxBuf[9]; 
		DataID = RxBuf[10];//�����ݰ�ʶ��PIDֵȡ��������֮ǰ��ֵ���Ա�ʾ�ź���������

	}
		
}

void SI24R1_SingalCheck(void)
{
	static uint8_t PreDataID = 250; 
	
	if(SI24R1_Controlflag)
	{
		if(FLY_UnLock && DataID == PreDataID)//�ɻ���ң�ضϿ�����
		{
			//�������䴦��
			//��Ƴ�������
		}
		PreDataID = DataID;
	}
}

