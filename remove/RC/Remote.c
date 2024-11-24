#include "SYS.h"                 

FLY_TYPE  FLY;



//接收分析飞机发送过来的数值
//void ReceiveDataAnalysis(void)
//{
//	if(RxBuf[0]==0xFF && RxBuf[11]==0xFF )
//	{
//		
//		FLY.Thr = ((RxBuf[1]<<8)|RxBuf[2]);
//		FLY.Yaw = ((s16)(RxBuf[3]<<8)|RxBuf[4])/100;
//		FLY.Pit = ((s16)(RxBuf[5]<<8)|RxBuf[6])/100;
//		FLY.Rol = ((s16)(RxBuf[7]<<8)|RxBuf[8])/100;
//		FLY.BattV = ((s16)(RxBuf[9]<<8)|RxBuf[10]);

//	}
//}

//void PackData(u8 firstByte)
//{
////	printf("ADC_Converonver1:%d\r\n",ADC_ConvertedValue[0]);
////	printf("ADC_Converonver12:%d\r\n",ADC_ConvertedValue[1]);
////	printf("ADC_Converonver13:%d\r\n",ADC_ConvertedValue[2]);
////	printf("ADC_Converonver14:%d\r\n",ADC_ConvertedValue[3]);
////	printf("+++++++++++++++++++++++++++++++++++++++++++++++\r\n");
//	
//	
//	ADC_Value[0] = 1500 + ((1000+ADC_ConvertedValue[0]*1000/4096) - ADC_Calibrator[0]);
//	ADC_Value[0] = ADC_ValueLimit(ADC_Value[0],1470,1530,1100,1900);

//	ADC_Value[1] = (ADC_ConvertedValue[1]*1000/4096); //油门0~1000;
//	
//	ADC_Value[2] = 1500 + ((1000+(ADC_ConvertedValue[2])*1000/4096) - ADC_Calibrator[2] );
//	ADC_Value[2] = ADC_ValueLimit(ADC_Value[2],1470,1530,1100,1900);
//	
//	ADC_Value[3] = 1500 + ((1000+(ADC_ConvertedValue[3])*1000/4096) - ADC_Calibrator[3] );
//	ADC_Value[3] = ADC_ValueLimit(ADC_Value[3],1470,1530,1100,1900);


////	printf("ADC1:%d\r\n",ADC_Value[0]);
////	printf("ADC2:%d\r\n",ADC_Value[1]);
////	printf("ADC3:%d\r\n",ADC_Value[2]);
////	printf("ADC4:%d\r\n",ADC_Value[3]);
////	
////	printf("ADC_Calibrator1:%d\r\n",ADC_Calibrator[0]);
////	printf("ADC_Calibrator2:%d\r\n",ADC_Calibrator[1]);
////	printf("ADC_Calibrator3:%d\r\n",ADC_Calibrator[2]);
////	printf("ADC_Calibrator4:%d\r\n",ADC_Calibrator[3]);
////	
////	printf("==================================\r\n");
//	
//	
//	//数据包识别PID自增，并且超过200时自动归零
//	if(dataPID>=200){
//		dataPID = 0;
//	}else{
//		dataPID++;
//	}
//	
////=========直接采用指针操作内存中的数值将16位转成8位，速度快，并且不会发生精度截取的现象，还要注意，STM32是小端地址============
//	packetData[0] = firstByte;					  //前导码
//	packetData[1] = ButtonMask;                   //按键
//	packetData[2] = *(((u8*)ADC_Value)+0);        //YAW角
//	packetData[3] = *(((u8*)ADC_Value)+1);
//	packetData[4] = *(((u8*)ADC_Value)+2);		  //油门数据打包
//	packetData[5] = *(((u8*)ADC_Value)+3);
//	packetData[6] = *(((u8*)ADC_Value)+4);        //ROLL角
//	packetData[7] = *(((u8*)ADC_Value)+5);
//	packetData[8] = *(((u8*)ADC_Value)+6);        //PITCH角
//	packetData[9] = *(((u8*)ADC_Value)+7);
//	packetData[10] = dataPID;					  //这个非常重要，这是防止飞机逃脱遥控的保证
//	packetData[11] = 0xa5;						  //校验码：1010 0101
//}

////====================================将处理好的数据进行打包===================================
////==============采用移位方式进行数据打包，注意，这种方式有可能会发生精度截取的现象=============
////	packetData[0] = firstByte;							//前导码
////	packetData[1] = ButtonMask;
////	packetData[2] = (u8)(ADC_ConvertedValue[0]>>8);
////	packetData[3] = ADC_ConvertedValue[0];
////	packetData[4] = (u8)(ADC_ConvertedValue[1]>>8);
////	packetData[5] = ADC_ConvertedValue[1];
////	packetData[6] = (u8)(ADC_ConvertedValue[2]>>8);
////	packetData[7] = ADC_ConvertedValue[2];
////	packetData[8] = (u8)(ADC_ConvertedValue[3]>>8);
////	packetData[9] = ADC_ConvertedValue[3];
////	packetData[10] = 0xa5;								//校验码：1010 0101
//	





