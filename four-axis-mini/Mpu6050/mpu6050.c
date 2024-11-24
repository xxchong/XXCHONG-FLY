
#include "sys.h"


int16_t MpuOffset[6] = {0};
static volatile int16_t *pMpu = (int16_t *)&MPU6050;

/****************************************************************************************
*@brief  
*@brief   
*@param[in]
*****************************************************************************************/
int8_t mpu6050_rest(void)
{
	if(IIC_Write_One_Byte(MPU6050_ADDRESS, PWR_MGMT_1, 0x80) == FAILED) return FAILED;	//复位
	delay_ms(20);
	return SUCCESS;
}
/****************************************************************************************
*@brief   
*@brief  
*@param[in]
*****************************************************************************************/
int8_t MpuInit(void) //初始化
{
	uint8_t date = SUCCESS;
	do
	{
		date = IIC_Write_One_Byte(MPU6050_ADDRESS, PWR_MGMT_1, 0x80);	//复位
		delay_ms(30);
		date += IIC_Write_One_Byte(MPU6050_ADDRESS, SMPLRT_DIV, 0x02); //陀螺仪采样率，0x00(500Hz)
		date += IIC_Write_One_Byte(MPU6050_ADDRESS, PWR_MGMT_1, 0x03);	//设置设备时钟源，陀螺仪Z轴
		date += IIC_Write_One_Byte(MPU6050_ADDRESS, CONFIGL, 0x03);   //低通滤波频率，0x03(42Hz)
		date += IIC_Write_One_Byte(MPU6050_ADDRESS, GYRO_CONFIG, 0x18);//+-2000deg/s
		date += IIC_Write_One_Byte(MPU6050_ADDRESS, ACCEL_CONFIG, 0x09);//+-4G
	}while(date != SUCCESS);
	
	date = IIC_Read_One_Byte(MPU6050_ADDRESS, 0x75);
	if(date!= MPU6050_PRODUCT_ID) return FAILED;
	else 
	{
		MpuGetOffset();
		return SUCCESS;
	}
}
/****************************************************************************************
*@brief    
*@brief   
*@param[in]
*****************************************************************************************/

#define  Acc_Read() IIC_read_Bytes(MPU6050_ADDRESS, 0X3B,buffer,6)
#define  Gyro_Read() IIC_read_Bytes(MPU6050_ADDRESS, 0x43,&buffer[6],6)



void MpuGetData(void) //读取陀螺仪数据加滤波
{
	uint8_t i;
	uint8_t buffer[12];

	Acc_Read();
	Gyro_Read();

	for(i=0;i<6;i++)
	{
		pMpu[i] = (((int16_t)buffer[i<<1] << 8) | buffer[(i<<1)+1])-MpuOffset[i];		
		if(i < 3)
		{
			static struct _1_ekf_filter ekf[3] = {{0.02,0,0,0,0.001,0.543},{0.02,0,0,0,0.001,0.543},{0.02,0,0,0,0.001,0.543}};	
			kalman_1(&ekf[i],(float)pMpu[i]);  //一维卡尔曼
			pMpu[i] = (int16_t)ekf[i].out;
		}
		if(i > 2)
		{	
			uint8_t k=i-3;
			const float factor = 0.15f;  //滤波因素			
			static float tBuff[3];		

			pMpu[i] = tBuff[k] = tBuff[k] * (1 - factor) + pMpu[i] * factor;                
		}
	}
}



/****************************************************************************************
*@brief   get mpu offset
*@brief   initial and cmd call this
*@param[in]
*****************************************************************************************/
void MpuGetOffset(void) //校准
{
	int32_t buffer[6]={0};
	int16_t i;  
	uint8_t k=30;
	const int8_t MAX_GYRO_QUIET = 5;
	const int8_t MIN_GYRO_QUIET = -5;	
	int16_t LastGyro[3] = {0};
	int16_t ErrorGyro[3];	
	
	memset(MpuOffset,0,12);		//set offset initial to zero
	MpuOffset[2] = 8192;		//set offset from the 8192  
	
//	TIM_ITConfig(TIM3 , TIM_IT_Update , DISABLE);
	
	while(k--)
	{
		do
		{
			delay_ms(10);
			MpuGetData();
			for(i=0;i<3;i++)
			{
				ErrorGyro[i] = pMpu[i+3] - LastGyro[i];
				LastGyro[i] = pMpu[i+3];	
			}			
		}while( (ErrorGyro[0] > MAX_GYRO_QUIET )|| (ErrorGyro[0] < MIN_GYRO_QUIET)	||
				(ErrorGyro[1] > MAX_GYRO_QUIET )|| (ErrorGyro[1] < MIN_GYRO_QUIET)	||
				(ErrorGyro[2] > MAX_GYRO_QUIET )|| (ErrorGyro[2] < MIN_GYRO_QUIET)	);
	}	

/*           throw first 100  group data and make 256 group average as offset                    */	
	for(i=0;i<356;i++)
	{		
		MpuGetData();
		if(100 <= i)
		{
			uint8_t k;
			for(k=0;k<6;k++)
			{
				buffer[k] += pMpu[k];
			}
		}
	}

	for(i=0;i<6;i++)
	{
		MpuOffset[i] = buffer[i]>>8;
	}
	
//	TIM_ITConfig(TIM3 , TIM_IT_Update , ENABLE );
}
/**************************************END OF FILE*************************************/








