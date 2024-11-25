#include "SYS.h"

unsigned char Property_Data[2];		//���ݱ�������
/*****************************************************
*�������ƣ�void ESP8266_Init(void)
*�������ܣ�ESP8266������ʼ��
*��ڲ�����void
*���ڲ�����void
*****************************************************/
void ESP8266_Init(void)
{
	esp8266_send_cmd("AT+CWMODE=1","OK",50);	//����WiFi����ģʽΪStationģʽ:����ESP8266ģ��ȥ�����Լ��ҵ�wifi,
															//�ֻ�Ҳ�����Լ��ҵ�wifiȻ��ʵ�����ֻ���WiFiģ���ͨ��,(�Լ���wifi�൱��һ���н�)
	//��Wifiģ������������
	esp8266_send_cmd("AT+RST","ready",20);
	delay_ms(1000);
	//��ģ���������Լ���·��
	esp8266_send_cmd("AT+CWJAP=\"TP-LINK_3104\",\"123456789000\"","WIFI GOT IP",300);
	//AT+MQTTUSERCFG - ���� MQTT �û�����
	esp8266_send_cmd(MQTT_set,"OK",100);
	//���� MQTT �ͻ��� ID
	esp8266_send_cmd(MQTT_Client,"OK",100);
	//����ָ�� MQTTЭ��
	esp8266_send_cmd(MQTT_Pass,"OK",300);
}

//��ESP8266��������
//cmd:���͵������ַ���;ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��;waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����);1,����ʧ��
u8 esp8266_send_cmd(char *cmd,char *ack,u16 waittime)
{
	u8 res=0; 
	USART2_RX_STA=0;
	u2_printf("%s\r\n",cmd);
	if(ack&&waittime)		
	{
		while(--waittime)	
		{
			delay_ms(10);
			if(USART2_RX_STA&0X8000)
			{
				if(esp8266_check_cmd(ack))
				{
					printf("ack:%s\r\n",(u8*)ack);
					break;
				}
					USART2_RX_STA=0;
			}
		}
		if(waittime==0)res=1; 
	}
	return res;
} 
//ESP8266���������,�����յ���Ӧ��
//str:�ڴ���Ӧ����
//����ֵ:0,û�еõ��ڴ���Ӧ����;����,�ڴ�Ӧ������λ��(str��λ��)

u8* esp8266_check_cmd(char *str)
{
	char *strx=0;
	if(USART2_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//��ӽ�����
		strx=strstr((const char*)USART2_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}

/*****************************************************
*�������ƣ�ESP8266_Send(char *property,int Data)
*�������ܣ����ƶ˷�������
*��ڲ�����char *property,int Data �����ԣ������ݣ�
*���ڲ�����void
*****************************************************/
void ESP8266_Send(char *property,int Data)
{
	USART2_RX_STA=0;
	u2_printf("AT+MQTTPUB=0,\"%s\",\"{\\\"params\\\":{\\\"%s\\\":%d}}\",1,0\r\n",post,property,Data);
}

/*****************************************************
*�������ƣ�ESP8266_Received(void)
*�������ܣ��ƶ˽�������
*��ڲ�����char *property,int Data �����ԣ������ݣ�
*���ڲ�����void
*****************************************************/
void ESP8266_Received(char *PRO)
{
	unsigned char *ret = 0;
	char *property = 0;
	unsigned char i;
	if(PRO == NULL);
	else if(USART2_RX_STA&0x8000)			//����2������һ֡����
	{
		ret = USART2_RX_BUF;
		if(ret!=0)
		{
			property = strstr((const char *)ret,(const char *)PRO);
			if(property!=NULL)
			{
				for(i=0;i<2;i++)
				{
					Property_Data[i] = *(property+12+i);
				}
				if(Property_Data[1] == '}')Property_Data[1]=0;
				USART2_RX_STA = 0;
			}
			else
			{
				USART2_RX_STA = 0;
			}
		}
		else
		{
			USART2_RX_STA = 0;	
		}
	}
}
