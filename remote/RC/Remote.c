#include "SYS.h"                 

FLY_TYPE  FLY;



//���շ����ɻ����͹�������ֵ
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

//	ADC_Value[1] = (ADC_ConvertedValue[1]*1000/4096); //����0~1000;
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
//	//���ݰ�ʶ��PID���������ҳ���200ʱ�Զ�����
//	if(dataPID>=200){
//		dataPID = 0;
//	}else{
//		dataPID++;
//	}
//	
////=========ֱ�Ӳ���ָ������ڴ��е���ֵ��16λת��8λ���ٶȿ죬���Ҳ��ᷢ�����Ƚ�ȡ�����󣬻�Ҫע�⣬STM32��С�˵�ַ============
//	packetData[0] = firstByte;					  //ǰ����
//	packetData[1] = ButtonMask;                   //����
//	packetData[2] = *(((u8*)ADC_Value)+0);        //YAW��
//	packetData[3] = *(((u8*)ADC_Value)+1);
//	packetData[4] = *(((u8*)ADC_Value)+2);		  //�������ݴ��
//	packetData[5] = *(((u8*)ADC_Value)+3);
//	packetData[6] = *(((u8*)ADC_Value)+4);        //ROLL��
//	packetData[7] = *(((u8*)ADC_Value)+5);
//	packetData[8] = *(((u8*)ADC_Value)+6);        //PITCH��
//	packetData[9] = *(((u8*)ADC_Value)+7);
//	packetData[10] = dataPID;					  //����ǳ���Ҫ�����Ƿ�ֹ�ɻ�����ң�صı�֤
//	packetData[11] = 0xa5;						  //У���룺1010 0101
//}

////====================================������õ����ݽ��д��===================================
////==============������λ��ʽ�������ݴ����ע�⣬���ַ�ʽ�п��ܻᷢ�����Ƚ�ȡ������=============
////	packetData[0] = firstByte;							//ǰ����
////	packetData[1] = ButtonMask;
////	packetData[2] = (u8)(ADC_ConvertedValue[0]>>8);
////	packetData[3] = ADC_ConvertedValue[0];
////	packetData[4] = (u8)(ADC_ConvertedValue[1]>>8);
////	packetData[5] = ADC_ConvertedValue[1];
////	packetData[6] = (u8)(ADC_ConvertedValue[2]>>8);
////	packetData[7] = ADC_ConvertedValue[2];
////	packetData[8] = (u8)(ADC_ConvertedValue[3]>>8);
////	packetData[9] = ADC_ConvertedValue[3];
////	packetData[10] = 0xa5;								//У���룺1010 0101
//	





