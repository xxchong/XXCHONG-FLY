#include "SYS.h"                  // Device header

uint8_t ESP8266_Init_SmartConfig(void)
{	

	ESP8266_Clear();
	printf("Start\r\n");
	int i=10;
	while( i>=0 ){ printf("i is %d",i);if(ESP8266_SendCmd("AT\r\n", "OK",200) ==0){break;}else{i--;if(i == 0)return 1;}}
	i=10;
	while( i>=0 ){ printf("i is %d",i);if(ESP8266_SendCmd("AT+CWMODE=1\r\n", "OK",200) ==0){break;}else{i--;}if(i == 0)return 1;}
	/* 设置WiFi模式
	 1：Station 模式
	 2：SoftAP 模式
	 3：SoftAP+Station 模式
	 */
	i=10;
	ESP8266_Clear();
	while( i>=0 ){ printf("i is %d",i);if(ESP8266_SendCmd("AT+CWSTARTSMART=3\r\n", "OK",200) ==0){break;}else{i--;}if(i == 0)return 1;}
	// 开启SmartConfig 

	
}


void ESP8266_Init_after_SmartConfig(void)
{
	int i=10;
	ESP8266_Clear();
	//关闭SmartConfig 
	while( i>=0 )
	{
		printf("i is %d",i);
		if(ESP8266_SendCmd("AT+CWSTOPSMART\r\n", "OK",200) ==0){break;}else{i--;}
	}
//	// 开启 DHCP 
//	i=10;
//	while( i>=0 ){ if(ESP8266_SendCmd("AT+CWDHCP=1,1\r\n", "OK",200)) ==0){break;}else{i--;}}
//	
}
