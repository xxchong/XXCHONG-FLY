#include "SYS.h"                  // Device header

uint8_t humiH,humiL,tempH,tempL;
uint8_t humi;
float temp;
uint8_t Flag_Con=1;
int flag=0;
int Motoflag=0;
uint8_t KeyNum=Button_ValNull;
uint8_t  func_index = 0;	//主程序此时所在程序的索引值
void (*current_operation_index)(void);
typedef struct
{
    u8 Current;	 	//当前状态索引号
    u8 Next; 			//向下一个
		u8 Last; 			//向上一个
    u8 Enter; 		//确定
    void (*current_operation)(void); //当前状态应该执行的操作 函数指针
} Menu_table;


uint8_t RData;
Menu_table table[30]=
{
	//索引，向下一个，上一个 ，确定
  {0,1,4,0,(&ShowTime)},		//一级界面（主页面） 
  {1,2,0,1,(&ShowWeather)},		//一级界面 天气
  {2,3,1,2,(&Show_HT11)},		//一级界面 温湿度 
	{3,13,2,5,(&Show_Motor_Icon)},   //一级界面 电机图标
  {4,0,13,6,(&Show_Set_Icon)},		//一级界面 设置
	
	{5,NULL,NULL,3,(&Control_Motor_Speed)},   //二级界面 控制电机函数

  {6,7,8,9,(&Set_SmartConfig_WiFI)},		//二级界面 智能配网
	{7,8,6,11,(&Set_Update_Weather)},   //二级界面 更新天气
	
	{8,6,7,4,(&Show_WIFI_Info)},   //二级界面 显示当前wifi名称
	
	{9,6,9,10,(&Confirm_Con)},   //三级界面 按下进行智能配网
	{10,NULL,NULL,NULL,(&Smart_Config)},   //智能配网具体执行函数
	
	{11,7,11,12,(&Confirm_Up)}, //三级界面 按下进行更新天气
	{12,NULL,NULL,NULL,(&Update_Weather)},//更新天气函数
	
	{13,4,3,14,(&Show_Flow_Led_Icon)}, //一级界面 流水灯图标
	{14,NULL,NULL,13,(&Flow_Led_Open)},//流水灯执行函数

};

void Show_Flow_Led_Icon(void)
{
		u8g2_FirstPage(&u8g2);
		do{
			u8g2_DrawFrame(&u8g2,0,0,128,64);
			u8g2_SetFont(&u8g2,u8g2_font_open_iconic_all_2x_t);			
			u8g2_DrawGlyph(&u8g2,55,32,0x0103);
			u8g2_SetFont(&u8g2,u8g2_font_unifont_t_symbols);
			u8g2_DrawStr(&u8g2,34,48,"Flow Led");
		}while(u8g2_NextPage(&u8g2));
}
void Flow_Led_Open(void)
{
		u8g2_FirstPage(&u8g2);
		do{
			u8g2_DrawFrame(&u8g2,0,0,128,64);
			u8g2_SetFont(&u8g2,u8g2_font_open_iconic_all_2x_t);			
			u8g2_DrawGlyph(&u8g2,55,32,0x0103);
			u8g2_SetFont(&u8g2,u8g2_font_6x10_tf);
			u8g2_DrawStr(&u8g2,18,48,"Flow Led Started");
		}while(u8g2_NextPage(&u8g2));
		Flow_Led();
}
void Show_Motor_Icon(void)
{	
	u8g2_FirstPage(&u8g2);
		do{
			u8g2_DrawFrame(&u8g2,0,0,128,64);
			u8g2_SetFont(&u8g2,u8g2_font_open_iconic_all_2x_t);			
			u8g2_DrawGlyph(&u8g2,55,32,0x0060);//shezhi
			u8g2_SetFont(&u8g2,u8g2_font_unifont_t_symbols);
			u8g2_DrawStr(&u8g2,44,48,"Motor");
		}while(u8g2_NextPage(&u8g2));
}

void Control_Motor_Speed()
{	
	int speed=0;
	while(flag >= 0)
	{
		uint8_t ret=Key_GetNum();	
		if(speed<-100 || speed >100)
			speed=0;
		if( ret== Button_ValLast &&  func_index ==5){
			speed -=20;
			u8g2_SetFont(&u8g2,u8g2_font_open_iconic_all_2x_t);			
			u8g2_DrawGlyph(&u8g2,20,59,0x0074);
			u8g2_SendBuffer(&u8g2);
			Delay_ms(20);
		}
		if(ret == Button_ValNext  &&  func_index ==5)
		{
			speed +=20;
			u8g2_SetFont(&u8g2,u8g2_font_open_iconic_all_2x_t);			
			u8g2_DrawGlyph(&u8g2,85,59,0x0077);
			u8g2_SendBuffer(&u8g2);
			Delay_ms(20);
		}

		if(ret == Button_ValEnter)
		{
			speed =0;
			flag=-1;
			func_index=3;
		}
			
		char speedBuf[20];
		sprintf(speedBuf,"Speed:%d",speed);
		u8g2_FirstPage(&u8g2);
		do{
			u8g2_SetFont(&u8g2,u8g2_font_6x10_tf);
			u8g2_DrawStr(&u8g2,10,42,"K1 To -");
			u8g2_DrawStr(&u8g2,70,42,"k2 To +");
			u8g2_DrawFrame(&u8g2,0,0,128,64);
			
			u8g2_SetFont(&u8g2,u8g2_font_unifont_t_symbols);
			u8g2_DrawStr(&u8g2,34,20,speedBuf);

		}while(u8g2_NextPage(&u8g2));

		Motor_SpeedControl(speed);

}
}
void ShowTime(void)
{
		
			MyRTC_ReadTime();
			char Date[20];
			char Time1[20];
			sprintf(Date,"%d-%d-%d",MyRTC_Time[0],MyRTC_Time[1],MyRTC_Time[2]);
			u8g2_FirstPage(&u8g2);
			do{
				sprintf(Time1,"%02d %02d",MyRTC_Time[3],MyRTC_Time[4]);
				u8g2_SetFont(&u8g2,u8g2_font_fub35_tn);
				u8g2_DrawStr(&u8g2,0,42,Time1);
				u8g2_DrawLine(&u8g2,0,45,128,45);
				u8g2_DrawLine(&u8g2,0,47,128,47);
				u8g2_DrawLine(&u8g2,0,0,128,0);
				u8g2_DrawLine(&u8g2,0,0,0,45);
				u8g2_DrawLine(&u8g2,127,0,127,45);
				u8g2_SetFont(&u8g2,u8g2_font_8x13_tf);
				u8g2_DrawStr(&u8g2,30,64,Date);
			}while(u8g2_NextPage(&u8g2));
	
			vTaskDelay(200);
			u8g2_FirstPage(&u8g2);
			do{

				u8g2_SetFont(&u8g2,u8g2_font_fub35_tn);
				u8g2_DrawStr(&u8g2,0,42,Time1);
				u8g2_DrawLine(&u8g2,0,45,128,45);
				u8g2_DrawLine(&u8g2,0,47,128,47);
				u8g2_DrawLine(&u8g2,0,0,128,0);
				u8g2_DrawLine(&u8g2,0,0,0,45);
				u8g2_DrawLine(&u8g2,127,0,127,45);
				u8g2_DrawStr(&u8g2,53,38,":");
				u8g2_SetFont(&u8g2,u8g2_font_8x13_tf);
				u8g2_DrawStr(&u8g2,30,64,Date);
				u8g2_SendBuffer(&u8g2);
			}while(u8g2_NextPage(&u8g2));
		
}





void ShowWeather(void)
{
			u8g2_FirstPage(&u8g2);
			do{
					u8g2_DrawFrame(&u8g2,0,0,128,64);
					u8g2_SetFont(&u8g2,u8g2_font_unifont_t_symbols);
					u8g2_DrawUTF8(&u8g2,35,14,"Weather");
					if(WifiFlag == 1 | UpdateWeather ==1 )
					{
						u8g2_SetFont(&u8g2,u8g2_font_MyChinese);
						u8g2_DrawUTF8(&u8g2,2 ,30,Weather_results[0].location.name);
						u8g2_DrawUTF8(&u8g2,2 ,45,Weather_results[0].now.temperature);
						u8g2_DrawUTF8(&u8g2,2 ,60,Weather_results[0].now.text);
						u8g2_DrawLine(&u8g2,0,16,127,16);
						u8g2_DrawLine(&u8g2,40,16,40,63);
						if(atoi(Weather_results[0].now.code) == 0)
						{
							u8g2_SetFont(&u8g2,u8g2_font_open_iconic_weather_4x_t);
							u8g2_DrawGlyph(&u8g2,70,60,0x0045);
						}else if((atoi(Weather_results[0].now.code) == 1))
						{
							u8g2_SetFont(&u8g2,u8g2_font_open_iconic_weather_4x_t);
							u8g2_DrawGlyph(&u8g2,70,60,0x0042);
						}else if((atoi(Weather_results[0].now.code) == 4 ) | (atoi(Weather_results[0].now.code) == 5 )  | (atoi(Weather_results[0].now.code) == 6)
							| (atoi(Weather_results[0].now.code) == 6 ) | (atoi(Weather_results[0].now.code) == 7 ) | (atoi(Weather_results[0].now.code) == 8 )) 
						{
							u8g2_SetFont(&u8g2,u8g2_font_open_iconic_weather_4x_t);
							u8g2_DrawGlyph(&u8g2,70,60,0x0041);
						}
						else if((atoi(Weather_results[0].now.code) == 9))
						{
							u8g2_SetFont(&u8g2,u8g2_font_open_iconic_weather_4x_t);
							u8g2_DrawGlyph(&u8g2,70,60,0x0040);
						}
						else if((atoi(Weather_results[0].now.code) == 10) | (atoi(Weather_results[0].now.code) == 11 )  | (atoi(Weather_results[0].now.code) == 12)
							| (atoi(Weather_results[0].now.code) == 13 ) | (atoi(Weather_results[0].now.code) == 14 ) | (atoi(Weather_results[0].now.code) == 15 ) 
						  | (atoi(Weather_results[0].now.code) == 16 ) | (atoi(Weather_results[0].now.code) == 17 )) 
						{
							u8g2_SetFont(&u8g2,u8g2_font_open_iconic_weather_4x_t);
							u8g2_DrawGlyph(&u8g2,70,60,0x0043);
						}else
						{
							u8g2_SetFont(&u8g2,u8g2_font_open_iconic_weather_4x_t);
							u8g2_DrawGlyph(&u8g2,70,60,0x0044);
						}}
					else
					{
							u8g2_SetFont(&u8g2,u8g2_font_unifont_t_symbols);
							u8g2_DrawStr(&u8g2,45,36,"N/A");
					}	
				}while(u8g2_NextPage(&u8g2));

}

void DHT11_Control_Motor(void)
{
	DHT11_Read_Data(&humiH,&humiL,&tempH,&tempL);
	if( humiH > 50 && humiH < 70)
	{
		Motor_SpeedControl(30);
	}else if( humiH > 70)
	{
		Motor_SpeedControl(60);
	}else
	{
		Motor_SpeedControl(0);
	}
	
}

void Show_HT11(void)
{

		
		char Buffer_HUMI[50];
		char Buffer_TEMP[50];
		sprintf(Buffer_HUMI,"%d.%d",humiH,humiL);
		sprintf(Buffer_TEMP,"%d.%d",tempH,tempL);
		u8g2_FirstPage(&u8g2);
		do{
				u8g2_DrawFrame(&u8g2,0,0,128,64);
				u8g2_SetFont(&u8g2,u8g2_font_6x10_tf);			
				u8g2_DrawStr(&u8g2,20,16,"Temp and humi");
				u8g2_SetFont(&u8g2,u8g2_font_unifont_t_symbols);
				u8g2_DrawStr(&u8g2, 10  ,54,Buffer_HUMI);
				u8g2_DrawStr(&u8g2, 70 ,54,Buffer_TEMP);
				u8g2_SetFont(&u8g2,u8g2_font_open_iconic_all_2x_t);			
				u8g2_DrawGlyph(&u8g2,80,40,0x00a6);//温度
				u8g2_DrawGlyph(&u8g2,20,40,0x00a8);//湿度
		}while(u8g2_NextPage(&u8g2));
}
void Show_Set_Icon(void)
{
		u8g2_FirstPage(&u8g2);
		do{
				u8g2_DrawFrame(&u8g2,0,0,128,64);
				u8g2_SetFont(&u8g2,u8g2_font_open_iconic_all_2x_t);			
				u8g2_DrawGlyph(&u8g2,55,32,0x0081);//shezhi
				u8g2_SetFont(&u8g2,u8g2_font_8x13_tf);			
				u8g2_DrawStr(&u8g2,50,48,"Set");//
			}while(u8g2_NextPage(&u8g2));
}
void Set_SmartConfig_WiFI(void)
{
		u8g2_FirstPage(&u8g2);
		do{
				u8g2_DrawFrame(&u8g2,0,0,128,64);
				u8g2_SetFont(&u8g2,u8g2_font_8x13_tf);
				u8g2_DrawStr(&u8g2,0,48,"SmartConfig WiFI");
				u8g2_SetFont(&u8g2,u8g2_font_open_iconic_all_2x_t);			
				u8g2_DrawGlyph(&u8g2,55,32,0x00f7);//shezhi
		}while(u8g2_NextPage(&u8g2));
}
void Set_Update_Weather(void)
{
		u8g2_FirstPage(&u8g2);
		do{
				u8g2_DrawFrame(&u8g2,0,0,128,64);
				u8g2_SetFont(&u8g2,u8g2_font_8x13_tf);
				u8g2_DrawStr(&u8g2,6,48,"Update Weather");
				u8g2_SetFont(&u8g2,u8g2_font_open_iconic_all_2x_t);			
				u8g2_DrawGlyph(&u8g2,55,32,0x00f3);//shezhi
		}while(u8g2_NextPage(&u8g2));
		
}


void Confirm_Up(void)
{
		u8g2_FirstPage(&u8g2);
		do{
				u8g2_DrawFrame(&u8g2,0,0,128,64);
				u8g2_SetFont(&u8g2,u8g2_font_6x10_tf);
				u8g2_DrawStr(&u8g2,12,32,"Press K3 to Update ");
		}while(u8g2_NextPage(&u8g2));

}
void Update_Weather(void)
{
	if(xTaskNotify(Task3_Handler,NULL,eNoAction) == pdPASS)
		{
			u8g2_FirstPage(&u8g2);
			do{
					u8g2_DrawFrame(&u8g2,0,0,128,64);
					u8g2_SetFont(&u8g2,u8g2_font_6x10_tf);
					u8g2_DrawStr(&u8g2,0,8,"Notify Update Task  ");
					u8g2_DrawStr(&u8g2,0,16,"success 3s will");
					u8g2_DrawStr(&u8g2,0,24,"3s will ");
					u8g2_DrawStr(&u8g2,0,32,"return setting");
				}while(u8g2_NextPage(&u8g2));
				vTaskDelay(3000);
				func_index=4;
		}
}



void Confirm_Con(void)
{
		u8g2_FirstPage(&u8g2);
		do{
				u8g2_DrawFrame(&u8g2,0,0,128,64);
				u8g2_SetFont(&u8g2,u8g2_font_6x10_tf);
				u8g2_DrawStr(&u8g2,0,32,"Press K3 to Connect");
		}while(u8g2_NextPage(&u8g2));

}



void Smart_Config(void)
{
		int i=50;
		uint8_t ret=0;
		int SucFlag=0;
		u8g2_FirstPage(&u8g2);
		do{
				u8g2_DrawFrame(&u8g2,0,0,128,64);
				u8g2_SetFont(&u8g2,u8g2_font_8x13_tf);
				u8g2_DrawStr(&u8g2,0,16,"Start SmartConfig");
		}while(u8g2_NextPage(&u8g2));	
		ret = ESP8266_Init_SmartConfig();
		Delay_ms(300);
		if(ret)
		{
			u8g2_FirstPage(&u8g2);
			do{
				u8g2_DrawFrame(&u8g2,0,0,128,64);
				u8g2_SetFont(&u8g2,u8g2_font_8x13_tf);
				u8g2_DrawStr(&u8g2,16,16,"AT  Fail");
				u8g2_DrawStr(&u8g2,16,32,"5s Will ");
				u8g2_DrawStr(&u8g2,0,48,"Return Setting");
			}while(u8g2_NextPage(&u8g2));			
			Delay_ms(5000);	
			func_index=3;
		 }else
		{
				while( i>= 0)
				{
					printf("i is %d\r\n ",i);
					if(strstr((char *)esp8266_buf, "smartconfig connected wifi"))
					{
						WifiFlag =1;
						SucFlag=1;
						ESP8266_Init_after_SmartConfig();
						break;
					}
					else{
						i--;
					}
						Delay_ms(1000);
				}
		}
			
			if(SucFlag == 1)
			{
				u8g2_FirstPage(&u8g2);
				do{
						u8g2_DrawFrame(&u8g2,0,0,128,64);
						u8g2_SetFont(&u8g2,u8g2_font_8x13_tf);
						u8g2_DrawStr(&u8g2,16,16,"smartconfig");
						u8g2_DrawStr(&u8g2,16,32," Success");
						u8g2_DrawStr(&u8g2,16,48,"5s Will ");
						u8g2_DrawStr(&u8g2,0,64,"Return Setting");
				}while(u8g2_NextPage(&u8g2));			
				Delay_ms(5000);	
				func_index=3;
			}else
			{
					u8g2_FirstPage(&u8g2);
					do{
							u8g2_DrawFrame(&u8g2,0,0,128,64);
							u8g2_SetFont(&u8g2,u8g2_font_8x13_tf);
							u8g2_DrawStr(&u8g2,0,16,"smartconfig ");
							u8g2_DrawStr(&u8g2,24,32,"Fail");
							u8g2_DrawStr(&u8g2,16,48,"5s Will ");
							u8g2_DrawStr(&u8g2,0,64,"Return Setting");
					}while(u8g2_NextPage(&u8g2));			
					Delay_ms(5000);	
					func_index=3;
			}
}

void Serial_Led(void)
{

}

void Show_WIFI_Info(void)
{
		if(WifiFlag == 1)
		{
			u8g2_FirstPage(&u8g2);
			do{
					u8g2_SetFont(&u8g2,u8g2_font_8x13_tf);
					u8g2_DrawStr(&u8g2,0,16,"Current WiFi:");
					u8g2_DrawStr(&u8g2,0,32,Wifiresult);
			}while(u8g2_NextPage(&u8g2));
		}else
		{
			u8g2_FirstPage(&u8g2);
			do{
					u8g2_SetFont(&u8g2,u8g2_font_8x13_tf);
					u8g2_DrawStr(&u8g2,0,16,"WiFi Disconnected");
			}while(u8g2_NextPage(&u8g2));
		}
}

void  Menu_key_set(void)
{
	DHT11_Read_Data(&humiH,&humiL,&tempH,&tempL);
//	DHT11_Control_Motor();
	uint8_t getNum=Key_GetNum();
  if(getNum == Button_ValNext )   
  { 
    func_index=table[func_index].Next;	//按键next按下后的索引号
  }
  if(getNum == Button_ValLast  )
  {
    func_index=table[func_index].Last;	//按键last按下后的索引号
  }
	if(getNum == Button_ValEnter)
  {
    func_index=table[func_index].Enter;	//按键enter按下后的索引号
	}
	if(func_index == 5)
	{
		Motoflag=1;
	}else
	{
		Motoflag=0;
	}
	if(Motoflag == 1)
	{
		flag= 2;
	}
  current_operation_index=table[func_index].current_operation;	//执行当前索引号所对应的功能函数
	(*current_operation_index)();//执行当前操作函数	
}

