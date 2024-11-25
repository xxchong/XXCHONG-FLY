#ifndef __MQTT_H
#define __MQTT_H


#define post "/sys/a1ZiRX0Nold/ESP8266/thing/event/property/post"
#define post_name "powerstate"

#define MQTT_set	"AT+MQTTUSERCFG=0,1,\"NULL\",\"ESP8266&a1ZiRX0Nold\",\"067c094501f86d26725e5b0c6596952f2b5d7d714e460cdc06e0904f1a513778\",0,0,\"\""
#define MQTT_Client "AT+MQTTCLIENTID=0,\"a1ZiRX0Nold.ESP8266|securemode=2\\,signmethod=hmacsha256\\,timestamp=1694783818474|\""
#define MQTT_Pass "AT+MQTTCONN=0,\"a1ZiRX0Nold.iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883,1"

#define WIFI_Name "TP-LINK_3104"
#define WIFI_Pass "123456789000"
extern unsigned char Property_Data[];		//数据保存数组

unsigned char ESP8266_Re(unsigned char *ACK_AT);
void ESP8266_Send(char *property,int Data);
void ESP8266_Received(char *PRO);
u8 esp8266_send_cmd(char *cmd,char *ack,u16 waittime);
u8* esp8266_check_cmd(char *str);


#endif
