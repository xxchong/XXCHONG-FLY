#include "SYS.h"                  // Device header

uint8_t Page = 0;
char text[50];
void MAIN_UI(void)
{
		
	u8g2_DrawUTF8(&u8g2, 40, 8, "XXCHONG");
	if(FLYDataRx_OK == 1)
	{
	
			
		FLYDataRx_OK=0;
			
		ReceiveDataAnalysis();

		
		sprintf(text,"State:");
		u8g2_DrawUTF8(&u8g2, 0,22, text);
		
		if(FLY.Lock == 1){
			
			u8g2_SetFont(&u8g2,u8g2_font_open_iconic_all_2x_t);
			u8g2_DrawGlyph(&u8g2,40,25,0x00cb);
			
				
		}else
		{
				u8g2_SetFont(&u8g2,u8g2_font_open_iconic_all_2x_t);
				u8g2_DrawGlyph(&u8g2,40,25,0x00ca);
			
		}

		u8g2_SetFont(&u8g2, u8g2_font_6x10_tf);
		
		
		
		sprintf(text,"Bat:%.2f",FLY.BattV);
		u8g2_DrawUTF8(&u8g2, 70, 22, text);
		
		
		if(FLY.Pit > 0)
		{
			sprintf(text,"Pitch:%03d",FLY.Pit);
			u8g2_DrawUTF8(&u8g2, 0, 44, text);
			
		}else
		{
			sprintf(text,"Pitch:-%03d",-FLY.Pit);
			u8g2_DrawUTF8(&u8g2, 0, 44, text);
			
		}
	
		
		if(FLY.Yaw > 0)
		{
			sprintf(text,"Yaw:%03d",FLY.Yaw);
			u8g2_DrawUTF8(&u8g2, 70, 44, text);
		}else
		{
			sprintf(text,"Yaw:-%03d",-FLY.Yaw);
			u8g2_DrawUTF8(&u8g2, 70, 44, text);
		}
		
		if(FLY.Rol > 0)
		{
			sprintf(text,"Roll:%03d",FLY.Rol);
			u8g2_DrawUTF8(&u8g2, 0, 58, text);
		}else
		{
			sprintf(text,"Roll:-%03d",-FLY.Rol);
			u8g2_DrawUTF8(&u8g2, 0, 58, text);
		}
		
		
		sprintf(text,"Thr:%04d",FLY.Thr);
		u8g2_DrawUTF8(&u8g2, 70, 58, text);
		u8g2_SendBuffer(&u8g2);
		
	}

	

}

void Second_Page(void)
{
	char text[40];	
	sprintf(text,"ADC_Value0:%04d",ADC_Value[0]);
	u8g2_DrawUTF8(&u8g2, 0, 16, text);
	sprintf(text,"ADC_Value1:%04d",ADC_Value[1]);
	u8g2_DrawUTF8(&u8g2, 0, 24, text);
	sprintf(text,"ADC_Value2:%04d",ADC_Value[2]);
	u8g2_DrawUTF8(&u8g2, 0, 32, text);
	sprintf(text,"ADC_Value3:%04d",ADC_Value[3]);
	u8g2_DrawUTF8(&u8g2, 0, 40, text);
	u8g2_SendBuffer(&u8g2);
	
}

void Calibration_UI(void)
{
	uint16_t i=6;
	u8g2Init(&u8g2);
	u8g2_SetFont(&u8g2, u8g2_font_6x10_tf); // ����
	u8g2_ClearBuffer(&u8g2);
	char Text[50];
	sprintf(Text,"Calibration");
	u8g2_DrawUTF8(&u8g2, 30, 8, Text);
	u8g2_SendBuffer(&u8g2);
	while(!ADC_CALIBRATOR_OK);

	if(ADC_CALIBRATOR_OK)
	{
		sprintf(Text,"Start Calibration");
		u8g2_DrawUTF8(&u8g2, 0, 16, Text);
		u8g2_SendBuffer(&u8g2);
	}
	
	while(ADC_CALIBRATOR_OK);
	sprintf(Text,"Calibration Finish");
	u8g2_DrawUTF8(&u8g2, 0, 24, Text);
	u8g2_SendBuffer(&u8g2);

}



