#include "SYS.h"
#include <stdio.h>
#include <stdarg.h>
#include "semphr.h" 

#define  TIMESTAMP 11
unsigned char backup_esp8266_buf[300];
unsigned char Json_buf[700];
//�����������
unsigned char Weather_buff[700]; 
char Wifiresult[50];
char WiFiName_buff[50];  
unsigned char Time_buff[700];   
u8 Size;
int timestamp_return;
uint8_t AlyFlag=0;

/*************************����2����DMAת��*******************/
volatile uint16_t Count = 0;			 /* ���ڽ��ܵ���Ϣ���� */
volatile uint8_t  DMA_RX_buffer[1024];   /* ���ڽ��ܻ��� */
volatile uint8_t DMA_RX_flag=0;      	 /* ���ڽ�����ɱ�־λ */
volatile uint8_t esp8266_buf[1024];		 /* DMA_RX_buffer->esp8266_buf */
/***********************************************************/

/*
*��������:USART2_IRQHandler
*��������:����2�����ж�DMA��������
*��ڲ���:��
*����ֵ:��
*/
void USART2_IRQHandler(void)
{
	volatile  uint8_t  ClearFlag;
	BaseType_t xHigherPriorityTaskWoken;
	/* ������տ����ж�  */
	if (USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
	{
		/* �����־���ȶ�SR���ٶ�DR */
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
				xSemaphoreGiveFromISR(xBinarySemaphore,&xHigherPriorityTaskWoken); //�ͷŶ�ֵ�ź���
			}
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);//�����Ҫ�Ļ�����һ�������л�
		}
		
	}
	

}

/*
*��������:ESP8266_Init
*��������:��ʼ��ESP8266
*��ڲ���:��
*����ֵ:��
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
*��������:MQTT_Init
*��������:MQTT���Ӽ�����Topic
*��ڲ���:��
*����ֵ:��
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
*��������:Get_Timestamp
*��������:��ȡʱ���
*��ڲ���:��
*����ֵ:��
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
*��������:Get_Day
*��������:��ȡʱ���
*��ڲ���:��
*����ֵ:��
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
*��������:Get_current_time
*��������:��ȡ����ʱ��
*��ڲ���:��
*����ֵ:��
*/
void Get_current_time(void)
{
	int i=10;
	
	WIFI.ESP8266_Clear();
	
	while(i >= 0)
	{ 
		if(WIFI.ESP8266_SendCmd("+++", "",500) == 0){break;}else{i--;}
	}     /*�˳�͸��ģʽ*/
	i=10;
	while(i >= 0)
	{ 
		if(WIFI.ESP8266_SendCmd("AT\r\n", "OK",100) == 0){break;}else{i--;}
	}     /*�˳�͸��ģʽ*/

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
	WIFI.ESP8266_SendData_Weather_OR_Time((u8 *)Time_GET,sizeof(Time_GET)); //����AT+CIPSEND  �Լ� Weather_GET  sizeof(Weather_GET)
	WIFI.ESP8266_GetIPD_GET(200,Time_buff);
	Delay_ms(100);
	WIFI.ESP8266_Clear();//�����������	
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
*��������: Get_current_weather
*��������:��ȡ��֪����
*��ڲ���:void
*����ֵ: void
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
	WIFI.ESP8266_SendData_Weather_OR_Time((u8 *)Weather_GET,sizeof(Weather_GET)); //����AT+CIPSEND  �Լ� Weather_GET  sizeof(Weather_GET)
	
	WIFI.ESP8266_GetIPD_GET(200,Weather_buff);
	WIFI.ESP8266_Clear();//�����������	
	i=10;
	while(i >= 0)
	{ 
		if(WIFI.ESP8266_SendCmd("+++", "",500) == 0){break;}else{i--; if(i == 0) return;}
	}     /*�˳�͸��ģʽ*/
	i=10;
	
		while(i >= 0)
	{ 
		if(WIFI.ESP8266_SendCmd("AT\r\n", "OK",100) == 0){break;}else{i--; if(i == 0) return;}
	}     /*�˳�͸��ģʽ*/


	i=10;
		while(i >= 0)
	{ 
		if(WIFI.ESP8266_SendCmd("AT+CIPCLOSE\r\n", "OK", 200) == 0){break;}else{i--; if(i == 0) return;}
	} 
}



/*
*��������:ESP8266_Clear
*��������:���buf����
*��ڲ���:��
*����ֵ:��
*/
void ESP8266_Clear(void)
{
	//void *memset(void *str, int c, size_t n) 
	//�����ַ� c��һ���޷����ַ��������� str ��ָ����ַ�����ǰ n ���ַ���
	memset((u8 *)esp8266_buf, 0, sizeof(esp8266_buf));

}

/*
*��������: ESP8266_GetIPD_GET
*��������:esp8266_buf ->  buff
*��ڲ���:void
*����ֵ: void
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
*��������:ESP8266_WaitRecive
*��������:�ȴ�����Ƿ�������
*��ڲ���:��
*����ֵ:REV_OK-�������		REV_WAIT-���ճ�ʱδ���
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
*��������:ESP8266_SendCmd
*��������:��������
*��ڲ���:cmd������
*         res����Ҫ���ķ���ָ��
*����ֵ: 0-�ɹ�	1-ʧ��
*/

uint8_t ESP8266_SendCmd(char *cmd, char *res, u16 time)
{	
	//strlen,���س���
	//���ݵ�ַ
	WIFI.ESP8266_Clear();
	Usart2_SendString((unsigned char *)cmd, strlen((const char *)cmd));
	while(time--)
	{
		if(WIFI.ESP8266_WaitRecive() == REV_OK)							//����յ�����
		{
			//C �⺯�� char *strstr(const char *haystack, const char *needle) 
			//���ַ��� haystack �в��ҵ�һ�γ����ַ��� needle ��λ�ã���������ֹ�� '\0'
			if(strstr((const char *)esp8266_buf, res) != NULL)		//����������ؼ���
			{
				WIFI.ESP8266_Clear();									//��ջ���
				return 0;
			}
		}
		Delay_ms(10);
	}
	return 1;
}

/*
*��������:ESP8266_SendData_Weather_OR_Time
*��������:���ͱ��Ļ�ȡ��������ʱ��
*��ڲ���:data������
*         len������
*����ֵ: 0-�ɹ�	1-ʧ��
*/
void ESP8266_SendData_Weather_OR_Time(unsigned char *data, unsigned short len)
{

	char cmdBuf[32];
	WIFI.ESP8266_Clear();								//��ս��ջ���
	sprintf(cmdBuf,"AT+CIPSEND\r\n");
	if(!WIFI.ESP8266_SendCmd(cmdBuf, ">", 200))				//�յ���>��ʱ���Է�������
	{
		Usart2_SendString(data,len);		//�����豸������������
	}
}
/*
*��������:ESP8266_SendData
*��������:�������� AT+CIPSEND
*��ڲ���:data������
*		  len������
*����ֵ:��
*/
void ESP8266_SendData(unsigned char *data, unsigned short len)
{
	char cmdBuf[32];
	WIFI.ESP8266_Clear();								//��ս��ջ���
	sprintf(cmdBuf, "AT+CIPSEND=%d\r\n", len);		//��������
	if(!WIFI.ESP8266_SendCmd(cmdBuf, ">", 200))				//�յ���>��ʱ���Է�������
	{
		Usart2_SendString(data,len);		//�����豸������������
	}
}





/*
*��������:Usart_SendString
*��������:�������ݷ���
*��ڲ���:str��Ҫ���͵�����
*				 len�����ݳ���
*����ֵ:��
*/
void Usart2_SendString(unsigned char *str, unsigned short len)
{
	unsigned short count;
	
	for(count = 0; count < len; count++)
	{
		USART_SendData(USART2, *str++);									//��������
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);		//�ȴ��������
	}
}

/*
*��������:ESP8266_SendCmd_WiFi_ID
*��������:��ȡWIFI��
*��ڲ���:��
*����ֵ:��
*/
uint8_t ESP8266_SendCmd_WiFi_ID(char *cmd, char *res, u16 time)
{	
	//strlen,���س���
	//���ݵ�ַ
	WIFI.ESP8266_Clear();
	Usart2_SendString((unsigned char *)cmd, strlen((const char *)cmd));
	while(time--)
	{
		if(ESP8266_WaitRecive() == REV_OK)							//����յ�����
		{
			//C �⺯�� char *strstr(const char *haystack, const char *needle) 
			//���ַ��� haystack �в��ҵ�һ�γ����ַ��� needle ��λ�ã���������ֹ�� '\0'
			
			if(strstr((const char *)esp8266_buf, res) != NULL)		//����������ؼ���
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
*��������:Show_WiFi_ID
*��������:��ȡWIFI��
*��ڲ���:��
*����ֵ:��
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
*��������:Get_WiFi_Name
*��������:�ӻ�ȡ����������ȡWIFI��
*��ڲ���:��
*����ֵ:��
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

