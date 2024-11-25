#include "SYS.h"

uint16_t MyRTC_Time[6];

void MyRTC_ReadTime(void)
{
	time_t time_cnt;
	struct tm time_date;
	time_cnt=RTC_GetCounter()+8*60*60;
	time_date=*localtime(&time_cnt);
	MyRTC_Time[0]=time_date.tm_year+1900;
	MyRTC_Time[1]=time_date.tm_mon+1;
	MyRTC_Time[2]=time_date.tm_mday;
	MyRTC_Time[3]=time_date.tm_hour;
	MyRTC_Time[4]=time_date.tm_min;
	MyRTC_Time[5]=time_date.tm_sec;

}

void MyRTC_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	PWR_BackupAccessCmd(ENABLE);

	if(BKP_ReadBackupRegister(BKP_DR1)!= 0xA5A5)
	{
		RCC_LSEConfig(RCC_LSE_ON);
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);//等待LSE启动
		//选择RTC时钟源
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
		RCC_RTCCLKCmd(ENABLE);
		//等待同步
		RTC_WaitForSynchro();
		//等待上一次写入操作完成
		RTC_WaitForLastTask();
		//配置预分频器,得到1hz
		RTC_SetPrescaler(32768-1);

		RTC_WaitForLastTask();
		//设置初始化时间
		RTC_SetCounter(0);
		RTC_WaitForLastTask();
		BKP_WriteBackupRegister(BKP_DR1,0xA5A5);

	}else
	{
		RTC_WaitForSynchro();
		RTC_WaitForLastTask();
	}

}

