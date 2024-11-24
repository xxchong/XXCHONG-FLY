#ifndef __WIFI_H
#define __WIFI_H

#define REV_OK		0	//接收完成标志
#define REV_WAIT	1	//接收未完成标志
#define WIFI_NAME_PWD  "AT+CWJAP=\"pixel 5a\",\"abc18813491718\"\r\n"  
//心知天气的API
#define Weather_TCP		"AT+CIPSTART=\"TCP\",\"api.seniverse.com\",80\r\n"
//心知天气GET报文
#define Weather_GET		"GET https://api.seniverse.com/v3/weather/now.json?key=SwXSXYOVnGU6ag5Ei&location=guangzhou&language=zh-Hans&unit=c\r\n"
#define TCP  "AT+CIPSTART=\"TCP\",\"192.168.38.19\",8080\r\n"
#define Time_TCP 			"AT+CIPSTART=\"TCP\",\"api.k780.com\",80\r\n"
#define Time_GET	    "GET http://api.k780.com/?app=life.time&appkey=71750&sign=6259c7fa791cd3da9b1806b312d31f7a&format=json\r\n"

extern volatile uint8_t esp8266_buf[1024];		 /* DMA_RX_buffer->esp8266_buf */
extern unsigned char backup_esp8266_buf[300];
extern unsigned char Time_buff[700]; 
extern  char WiFiName_buff[50]; 
extern int timestamp_return;
extern char Wifiresult[50];
extern uint8_t AlyFlag;
extern unsigned char Json_buf[700];
extern volatile uint8_t  DMA_RX_buffer[1024];   /* 串口接受缓存 */

typedef struct{
    char *WIFI_NAME;
    char *WIFI_PWD;
    uint8_t (*MQTT_Init)(void);
    void (*ESP8266_Clear)(void);
    void (*ESP8266_SendData)(unsigned char *data, unsigned short len);
    uint8_t (*ESP8266_SendCmd)(char *cmd, char *res, u16 time); // 注意这里的括号
    unsigned char * (*ESP8266_GetIPD_GET)(unsigned short timeOut,u8 *buff);
    void (*Get_current_weather)(void);
    void (*ESP8266_SendData_Weather_OR_Time)(unsigned char *data, unsigned short len);
    void (*Show_WiFi_ID)(unsigned short timeOut);
    uint8_t (*ESP8266_SendCmd_WiFi_ID)(char *cmd, char *res, u16 time);
    void (*Get_WiFi_Name)(void);
		_Bool (*ESP8266_WaitRecive)(void)
} WIFI_t;


uint8_t MQTT_Init(void);
void ESP8266_Init(void);
void ESP8266_Clear(void);
void ESP8266_SendData(unsigned char *data, unsigned short len);
void Usart2_SendString(unsigned char *str, unsigned short len);
uint8_t ESP8266_SendCmd(char *cmd, char *res, u16 time);
unsigned char *ESP8266_GetIPD_GET(unsigned short timeOut,u8 *buff);
void Get_current_weather(void);
void ESP8266_SendData_Weather_OR_Time(unsigned char *data, unsigned short len);
void Get_current_time(void);
void Get_Timestamp(void);
int Get_Day(char *d);
void Show_WiFi_ID(unsigned short timeOut);
uint8_t ESP8266_SendCmd_WiFi_ID(char *cmd, char *res, u16 time);
void get_content_between_quotes(const char *str, char *result) ;
void Get_WiFi_Name(void);
void u2_printf(char *fmt,...);
_Bool ESP8266_WaitRecive(void);

#endif

