#include "SYS.h"
#include <stdio.h>
#include <stdarg.h>
#include "semphr.h" 

#define  TIMESTAMP 11
unsigned char backup_esp8266_buf[300];
unsigned char Json_buf[700];
//存放天气数据
unsigned char Weather_buff[700]; 
char Wifiresult[50];
char WiFiName_buff[50];  
unsigned char Time_buff[700];   
u8 Size;
int timestamp_return;
uint8_t AlyFlag=0;

/*************************串口2空闲DMA转运*******************/
volatile uint16_t Count = 0;			 /* 串口接受的消息数量 */
volatile uint8_t  DMA_RX_buffer[1024];   /* 串口接受缓存 */
volatile uint8_t DMA_RX_flag=0;      	 /* 串口接受完成标志位 */
volatile uint8_t esp8266_buf[1024];		 /* DMA_RX_buffer->esp8266_buf */
/***********************************************************/

/*
*函数名称:USART2_IRQHandler
*函数功能:串口2空闲中断DMA接收数据
*入口参数:无
*返回值:无
*/
void USART2_IRQHandler(void)
{
	volatile  uint8_t  ClearFlag;
	BaseType_t xHigherPriorityTaskWoken;
	/* 处理接收空闲中断  */
	if (USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
	{
		/* 清除标志：先读SR，再读DR */
		ClearFlag = USART2->SR;
		ClearFlag = USART2->DR;
		DMA_RX_flag=1;
		DMA_Cmd(DMA1_Channel6, DISABLE);
        Count = 512 - DMA_GetCurrDataCounter(DMA1_Channel6);
		printf("Count:%d\r\n",Count);
        for (int i = 0; i < Count; i++)
        {
            esp8266_buf[i] = DMA_RX_buffer[i];
			printf("%c",DMA_RX_buffer[i]);
        }
		printf("\r\n");
        memset((u8 *)DMA_RX_buffer, 0, sizeof(DMA_RX_buffer));
        DMA_SetCurrDataCounter(DMA1_Channel6, 512);
        DMA_Cmd(DMA1_Channel6, ENABLE);
		if(strstr((const char*)esp8266_buf,"LightSwitch")!=NULL)
		{
			if(xBinarySemaphore!=NULL)
			{
				xSemaphoreGiveFromISR(xBinarySemaphore,&xHigherPriorityTaskWoken); //释放二值信号量
			}
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);//如果需要的话进行一次任务切换
		}
		
	}
	

}

/*
*函数名称:ESP8266_Init
*函数功能:初始化ESP8266
*入口参数:无
*返回值:无
*/
void ESP8266_Init(void)
{
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	
	WIFI.ESP8266_Clear=ESP8266_Clear;
	WIFI.ESP8266_GetIPD_GET=ESP8266_GetIPD_GET;
	WIFI.ESP8266_SendCmd=ESP8266_SendCmd;
	WIFI.ESP8266_SendCmd_WiFi_ID=ESP8266_SendCmd_WiFi_ID;
	WIFI.ESP8266_SendData=ESP8266_SendData;
	WIFI.ESP8266_SendData_Weather_OR_Time=ESP8266_SendData_Weather_OR_Time;
	WIFI.Get_current_weather=Get_current_weather;
	WIFI.Get_WiFi_Name=Get_WiFi_Name;
	WIFI.MQTT_Init=MQTT_Init;
	WIFI.Show_WiFi_ID=Show_WiFi_ID;
	WIFI.ESP8266_WaitRecive=ESP8266_WaitRecive;
	WIFI.WIFI_NAME="pixel 5a";
	WIFI.WIFI_PWD="abc18813491718";
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_6);
	Delay_ms(20);
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);
	Delay_ms(200);
	GPIO_SetBits(GPIOA,GPIO_Pin_6);
	WIFI.ESP8266_Clear();
}
/*
*函数名称:MQTT_Init
*函数功能:MQTT连接及订阅Topic
*入口参数:无
*返回值:无
*/
uint8_t MQTT_Init(void)
{
	int i=10;
	WIFI.ESP8266_Clear();
	while(i >= 0)
	{ 
		if(WIFI.ESP8266_SendCmd("AT+CIPSNTPCFG=1,8,\"ntp1.aliyun.com\"\r\n", "OK",200) == 0){break;}else{i--;if(  i == 0) return 1;}
	}     
	i=10;
	Delay_ms(500);
	while(i >= 0)
	{ 
		if(WIFI.ESP8266_SendCmd("AT+MQTTUSERCFG=0,1,\"NULL\",\"ESP8266&k0404bKWwjP\",\"e17560347e8db993d09ceb4c5face7a7fbc07eb7cd7d4ab69234c04e56bebe0b\",0,0,\"\"\r\n", "OK",200) == 0){break;}else{i--;if(  i == 0) return 2;}
	}    

	i=10;
	Delay_ms(500);

		while(i >= 0)
	{ 
		if(WIFI.ESP8266_SendCmd("AT+MQTTCLIENTID=0,\"k0404bKWwjP.ESP8266|securemode=2\\,signmethod=hmacsha256\\,timestamp=1705499202156|\"\r\n", "OK",200) == 0){break;}else{i--;if(  i == 0) return 3;}
	}     
	
	i=10;
	Delay_ms(500);

	while(i >= 0)
	{ 
		if(WIFI.ESP8266_SendCmd("AT+MQTTCONN=0,\"k0404bKWwjP.iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883,1\r\n", "OK",200) == 0){break;}else{i--;if(  i == 0) return 4;}
	}     
//	i=10;
//		while(i >= 0)
//	{ 
//		if(WIFI.ESP8266_SendCmd("AT+MQTTSUB=0,\"/sys/k0404bKWwjP/ESP8266/thing/service/property/set\",1\r\n", "OK",200) == 0){break;}else{i--;if(  i == 0) return 5;}
//	}    
//	/k0404bKWwjP/ESP8266/user/get
	i=10;
	while(i >= 0)
	{ 
		if(WIFI.ESP8266_SendCmd("AT+MQTTSUB=0,\"/k0404bKWwjP/ESP8266/user/get\",0\r\n", "OK",200) == 0){break;}else{i--;if(  i == 0) return 6;}
	}    

	i=10;
	while(i >= 0)
	{ 
		if(WIFI.ESP8266_SendCmd("AT+MQTTUNSUB=0,\"/sys/k0404bKWwjP/ESP8266/thing/event/property/post_reply\"\r\n", "OK",200) == 0){break;}else{i--;if(  i == 0) return 6;}
	}  

	return 0;
}	

/*
*函数名称:Get_Timestamp
*函数功能:获取时间戳
*入口参数:无
*返回值:无
*/
void Get_Timestamp(void)
{
	char *data_pt;
	char *Timestamp;
	data_pt = strstr((const char *)Time_buff, (const char *)"timestamp");
	if(data_pt != NULL)
	{
		Timestamp=data_pt+TIMESTAMP;
		timestamp_return=Get_Day(Timestamp);
	}
}
/*
*函数名称:Get_Day
*函数功能:获取时间戳
*入口参数:无
*返回值:无
*/
int Get_Day(char *y)
{
	int return_timestamp_temp;
    char *timestamp_temp;
    char timestamp_buf[10];
    char i;
    timestamp_temp = y;
    for(i = 0; i < 10; i++)
    {
      timestamp_buf[i] = *timestamp_temp++;
    }
     return_timestamp_temp= atoi(&timestamp_buf[0]);
		
		return return_timestamp_temp;

}
/*
*函数名称:Get_current_time
*函数功能:获取网络时间
*入口参数:无
*返回值:无
*/
void Get_current_time(void)
{
	int i=10;
	
	WIFI.ESP8266_Clear();
	
	while(i >= 0)
	{ 
		if(WIFI.ESP8266_SendCmd("+++", "",500) == 0){break;}else{i--;}
	}     /*退出透传模式*/
	i=10;
	while(i >= 0)
	{ 
		if(WIFI.ESP8266_SendCmd("AT\r\n", "OK",100) == 0){break;}else{i--;}
	}     /*退出透传模式*/

	i=10;
	while(i >= 0)
	{ 
		if(WIFI.ESP8266_SendCmd(Time_TCP, "CONNECT",100) == 0){break;}else{i--;}
	}     
	i=10;
	while(i >= 0)
	{ 
		if(WIFI.ESP8266_SendCmd("AT+CIPMODE=1\r\n", "OK", 200) == 0){break;}else{i--;}
	}    
	WIFI.ESP8266_SendData_Weather_OR_Time((u8 *)Time_GET,sizeof(Time_GET)); //发送AT+CIPSEND  以及 Weather_GET  sizeof(Weather_GET)
	WIFI.ESP8266_GetIPD_GET(200,Time_buff);
	Delay_ms(100);
	WIFI.ESP8266_Clear();//清除缓存数据	
	i=10;
	while(i >= 0)
	{ 
		if(WIFI.ESP8266_SendCmd("+++", "",500) == 0){break;}else{i--;}
	}     
	i=10;
	
	while(i >= 0)
	{ 
		if(WIFI.ESP8266_SendCmd("AT\r\n", "OK",100) == 0){break;}else{i--;}
	}     
	i=10;
	while(i >= 0)
	{ 
		if(WIFI.ESP8266_SendCmd("AT+CIPCLOSE\r\n", "OK", 200) == 0){break;}else{i--;}
	}     

}

/*
*函数名称: Get_current_weather
*函数功能:获取心知天气
*入口参数:void
*返回值: void
*/
void Get_current_weather(void)
{
	int i=10;
	WIFI.ESP8266_Clear();
	while(i >= 0)
	{ 
		if(WIFI.ESP8266_SendCmd("+++", "",500) == 0){break;}else{i--; if(i == 0) return;}
	}     
	i=10;
	while(i >= 0)
	{ 
		if(WIFI.ESP8266_SendCmd("AT\r\n", "OK",500) == 0){break;}else{i--; if(i == 0) return;}
	}     
	i=10;	
	while(i >= 0)
	{ 
		if(WIFI.ESP8266_SendCmd(Weather_TCP, "CONNECT", 200) == 0){break;}else{i--; if(i == 0) return;}
	}    
	i=10;
	while(i >= 0)
	{ 
		if(WIFI.ESP8266_SendCmd("AT+CIPMODE=1\r\n", "OK", 500) == 0){break;}else{i--; if(i == 0) return;}
	}  
	WIFI.ESP8266_SendData_Weather_OR_Time((u8 *)Weather_GET,sizeof(Weather_GET)); //发送AT+CIPSEND  以及 Weather_GET  sizeof(Weather_GET)
	
	WIFI.ESP8266_GetIPD_GET(200,Weather_buff);
	WIFI.ESP8266_Clear();//清除缓存数据	
	i=10;
	while(i >= 0)
	{ 
		if(WIFI.ESP8266_SendCmd("+++", "",500) == 0){break;}else{i--; if(i == 0) return;}
	}     /*退出透传模式*/
	i=10;
	
		while(i >= 0)
	{ 
		if(WIFI.ESP8266_SendCmd("AT\r\n", "OK",100) == 0){break;}else{i--; if(i == 0) return;}
	}     /*退出透传模式*/


	i=10;
		while(i >= 0)
	{ 
		if(WIFI.ESP8266_SendCmd("AT+CIPCLOSE\r\n", "OK", 200) == 0){break;}else{i--; if(i == 0) return;}
	} 
}



/*
*函数名称:ESP8266_Clear
*函数功能:清空buf缓存
*入口参数:无
*返回值:无
*/
void ESP8266_Clear(void)
{
	//void *memset(void *str, int c, size_t n) 
	//复制字符 c（一个无符号字符）到参数 str 所指向的字符串的前 n 个字符。
	memset((u8 *)esp8266_buf, 0, sizeof(esp8266_buf));

}

/*
*函数名称: ESP8266_GetIPD_GET
*函数功能:esp8266_buf ->  buff
*入口参数:void
*返回值: void
*/
unsigned char *ESP8266_GetIPD_GET(unsigned short timeOut,u8 *buff)
{
	do
	{
		Delay_ms(5);													
	} while(timeOut--);
	strcpy((char*)buff,(char*)esp8266_buf);
	return buff;														
}




/*
*函数名称:ESP8266_WaitRecive
*函数功能:等待检测是否接收完成
*入口参数:无
*返回值:REV_OK-接收完成		REV_WAIT-接收超时未完成
*/
_Bool ESP8266_WaitRecive(void)
{
	if(DMA_RX_flag == 1)
	{
		DMA_RX_flag=0;
		return REV_OK;
	}
	return REV_WAIT;
}


/*
*函数名称:ESP8266_SendCmd
*函数功能:发送命令
*入口参数:cmd：命令
*         res：需要检查的返回指令
*返回值: 0-成功	1-失败
*/

uint8_t ESP8266_SendCmd(char *cmd, char *res, u16 time)
{	
	//strlen,返回长度
	//传递地址
	WIFI.ESP8266_Clear();
	Usart2_SendString((unsigned char *)cmd, strlen((const char *)cmd));
	while(time--)
	{
		if(WIFI.ESP8266_WaitRecive() == REV_OK)							//如果收到数据
		{
			//C 库函数 char *strstr(const char *haystack, const char *needle) 
			//在字符串 haystack 中查找第一次出现字符串 needle 的位置，不包含终止符 '\0'
			if(strstr((const char *)esp8266_buf, res) != NULL)		//如果检索到关键词
			{
				WIFI.ESP8266_Clear();									//清空缓存
				return 0;
			}
		}
		Delay_ms(10);
	}
	return 1;
}

/*
*函数名称:ESP8266_SendData_Weather_OR_Time
*函数功能:发送报文获取天气或者时间
*入口参数:data：数据
*         len：长度
*返回值: 0-成功	1-失败
*/
void ESP8266_SendData_Weather_OR_Time(unsigned char *data, unsigned short len)
{

	char cmdBuf[32];
	WIFI.ESP8266_Clear();								//清空接收缓存
	sprintf(cmdBuf,"AT+CIPSEND\r\n");
	if(!WIFI.ESP8266_SendCmd(cmdBuf, ">", 200))				//收到‘>’时可以发送数据
	{
		Usart2_SendString(data,len);		//发送设备连接请求数据
	}
}
/*
*函数名称:ESP8266_SendData
*函数功能:发送数据 AT+CIPSEND
*入口参数:data：数据
*		  len：长度
*返回值:无
*/
void ESP8266_SendData(unsigned char *data, unsigned short len)
{
	char cmdBuf[32];
	WIFI.ESP8266_Clear();								//清空接收缓存
	sprintf(cmdBuf, "AT+CIPSEND=%d\r\n", len);		//发送命令
	if(!WIFI.ESP8266_SendCmd(cmdBuf, ">", 200))				//收到‘>’时可以发送数据
	{
		Usart2_SendString(data,len);		//发送设备连接请求数据
	}
}





/*
*函数名称:Usart_SendString
*函数功能:串口数据发送
*入口参数:str：要发送的数据
*				 len：数据长度
*返回值:无
*/
void Usart2_SendString(unsigned char *str, unsigned short len)
{
	unsigned short count;
	
	for(count = 0; count < len; count++)
	{
		USART_SendData(USART2, *str++);									//发送数据
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);		//等待发送完成
	}
}

/*
*函数名称:ESP8266_SendCmd_WiFi_ID
*函数功能:提取WIFI名
*入口参数:无
*返回值:无
*/
uint8_t ESP8266_SendCmd_WiFi_ID(char *cmd, char *res, u16 time)
{	
	//strlen,返回长度
	//传递地址
	WIFI.ESP8266_Clear();
	Usart2_SendString((unsigned char *)cmd, strlen((const char *)cmd));
	while(time--)
	{
		if(ESP8266_WaitRecive() == REV_OK)							//如果收到数据
		{
			//C 库函数 char *strstr(const char *haystack, const char *needle) 
			//在字符串 haystack 中查找第一次出现字符串 needle 的位置，不包含终止符 '\0'
			
			if(strstr((const char *)esp8266_buf, res) != NULL)		//如果检索到关键词
			{
				WIFI.ESP8266_GetIPD_GET(200,(u8 *)WiFiName_buff);
				return 0;
			}
		}
		
		Delay_ms(10);
	}
	return 1;
}


/*
*函数名称:Show_WiFi_ID
*函数功能:获取WIFI名
*入口参数:无
*返回值:无
*/

void Show_WiFi_ID(unsigned short timeOut)
{
	
	WIFI.ESP8266_Clear();
	int i=10;
	while(i >= 0)
	{ 
		if(WIFI.ESP8266_SendCmd("AT\r\n", "OK",100) == 0){break;}else{i--;}
	}    
	 i=10;
	while(i >= 0)
	{ 
		if(WIFI.ESP8266_SendCmd_WiFi_ID("AT+CWJAP?\r\n", "OK",100) == 0){break;}else{i--;}
	}
	WIFI.Get_WiFi_Name();
}

/*
*函数名称:Get_WiFi_Name
*函数功能:从获取的数据中提取WIFI名
*入口参数:无
*返回值:无
*/


void Get_WiFi_Name(void)
{
	char *data_pt;
	char *NameStr;
	data_pt = strstr((const char *)WiFiName_buff, (const char *)"+CWJAP:");
	if(data_pt != NULL)
	{
		NameStr=data_pt;
		get_content_between_quotes(NameStr,Wifiresult);
	}
}
void get_content_between_quotes(const char *str, char *result) {
    int start = -1;
    int end = -1;
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == '\"' && start == -1) {
            start = i + 1;
        } else if (str[i] == '\"' && start != -1) {
            end = i;
            break;
        }
    }
    if (start != -1 && end != -1) {
        strncpy(Wifiresult, str + start, end - start);
        Wifiresult[end - start] = '\0';
    } else {
        Wifiresult[0] = '\0';
    }
}

