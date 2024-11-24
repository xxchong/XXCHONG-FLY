#include "sys.h"

/////////////////////////////////////////////////////////////////////////////////////
//���ݲ�ֺ궨�壬�ڷ��ʹ���1�ֽڵ���������ʱ������int16��float�ȣ���Ҫ�����ݲ�ֳɵ����ֽڽ��з���
//#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)		) )
//#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
//#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
//#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )

dt_flag_t f;					//��Ҫ�������ݵı�־
uint8_t data_to_send[50];	//�������ݻ���



//extern MPU6050_st Mpu6050_Data;
extern BATT_TYPE BAT;
extern RC_TYPE RC;
/////////////////////////////////////////////////////////////////////////////////////
//Data_Exchange��������������ݷ������󣬱�����ʵ��ÿ5ms����һ�δ�������������λ�������ڴ˺�����ʵ��
//�˺���Ӧ���û�ÿ1ms����һ��
void ANO_DT_Data_Exchange(void)
{
	static uint8_t status_cnt 	= 0;
	static uint8_t senser_cnt 	= 0;
	static uint8_t rcdata_cnt 	= 0;
	static uint8_t motopwm_cnt	= 0;
	static uint8_t power_cnt	= 0;
	status_cnt++;
	senser_cnt++;
	rcdata_cnt++;
	motopwm_cnt++;
	power_cnt++;
	if(status_cnt>=55)
	{
		status_cnt = 0;
		f.send_status = 1;
	}
	if(senser_cnt>=5)
	{
		senser_cnt = 0;
		f.send_senser = 1;
	}
	if(rcdata_cnt>=20)
	{
		rcdata_cnt = 0;
		f.send_rcdata = 1;
	}
	if(motopwm_cnt>=35)
	{
		motopwm_cnt = 0;
		f.send_motopwm = 1;
	}
	if(power_cnt>=60)
	{
		power_cnt = 0;
		f.send_power = 1;
	}

	if(f.send_version)
	{
		f.send_version = 0;
		ANO_DT_Send_Version(4,300,100,400,0);
	}

	 if(f.send_status)
	{
		f.send_status = 0;
		ANO_DT_Send_Status(Angle.roll,Angle.pitch,Angle.yaw,1,0,1);
	}	

	else if(f.send_senser)
	{

		
		f.send_senser = 0;
		ANO_DT_Send_Senser(MPU6050.accX,MPU6050.accY,MPU6050.accZ,
												MPU6050.gyroX,MPU6050.gyroY,MPU6050.gyroZ,
										0,0,0);
	}	

	else if(f.send_rcdata)
	{
		f.send_rcdata = 0;
		ANO_DT_Send_RCData(RC.THROTTLE,RC.YAW,RC.ROLL,RC.PITCH,0,0,0,0,0,0);

	}	
	
	else if(f.send_motopwm)
	{
		f.send_motopwm = 0;
		ANO_DT_Send_MotoPWM(1,2,3,4,5,6,7,8);
	}	

	else if(f.send_power)
	{
		f.send_power = 0;
		ANO_DT_Send_Power(BAT.BattMeasureV * 100,BAT.BattRealV * 100 );
	}

	
	 else if(f.send_pid1)
	{
		f.send_pid1 = 0;
		ANO_DT_Send_PID(1,
						pidRateY.kp,pidRateY.ki,pidRateY.kd,
						pidRateX.kp,pidRateX.ki,pidRateX.kd,
						pidRateZ.kp,pidRateZ.ki,pidRateZ.kd);
	}	
	else if(f.send_pid2)
	{
		f.send_pid2 = 0;
		ANO_DT_Send_PID(2,
		pidRoll.kp,pidRoll.ki,pidRoll.kd,
		pidPitch.kp,pidPitch.ki,pidPitch.kd,
		pidYaw.kp,pidYaw.ki,pidYaw.kd
		);
	}

}



static void ANO_DT_Send_Check(u8 head, u8 check_sum)
{
	data_to_send[0]=0xAA;
	data_to_send[1]=0xAA;
	data_to_send[2]=0xEF;
	data_to_send[3]=2;
	data_to_send[4]=head;
	data_to_send[5]=check_sum;
	
	
	u8 sum = 0;
	for(u8 i=0;i<6;i++)
		sum += data_to_send[i];
	data_to_send[6]=sum;

	USART1_DMA_Send(data_to_send, 7);
}

/////////////////////////////////////////////////////////////////////////////////////
//Data_Receive_Prepare������Э��Ԥ����������Э��ĸ�ʽ�����յ������ݽ���һ�θ�ʽ�Խ�������ʽ��ȷ�Ļ��ٽ������ݽ���
//��ֲʱ���˺���Ӧ���û���������ʹ�õ�ͨ�ŷ�ʽ���е��ã����紮��ÿ�յ�һ�ֽ����ݣ�����ô˺���һ��
//�˺������������ϸ�ʽ������֡�󣬻����е������ݽ�������
void ANO_DT_Data_Receive_Prepare(u8 data)
{
	static u8 RxBuffer[50];
	static u8 _data_len = 0,_data_cnt = 0;
	static u8 state = 0;
	
	if(state==0&&data==0xAA)
	{
		state=1;
		RxBuffer[0]=data;
	}
	else if(state==1&&data==0xAF)
	{
		state=2;
		RxBuffer[1]=data;
	}
	else if(state==2&&data<0XF1)
	{
		state=3;
		RxBuffer[2]=data;
	}
	else if(state==3&&data<50)
	{
		state = 4;
		RxBuffer[3]=data;
		_data_len = data;
		_data_cnt = 0;
	}
	else if(state==4&&_data_len>0)
	{
		_data_len--;
		RxBuffer[4+_data_cnt++]=data;
		if(_data_len==0)
			state = 5;
	}
	else if(state==5)
	{
		state = 0;
		RxBuffer[4+_data_cnt]=data;
		ANO_DT_Data_Receive_Anl(RxBuffer,_data_cnt+5);
	}
	else
		state = 0;
}
/////////////////////////////////////////////////////////////////////////////////////
////Data_Receive_Anl������Э�����ݽ������������������Ƿ���Э���ʽ��һ������֡���ú��������ȶ�Э�����ݽ���У��
////У��ͨ��������ݽ��н�����ʵ����Ӧ����
////�˺������Բ����û����е��ã��ɺ���Data_Receive_Prepare�Զ�����
void ANO_DT_Data_Receive_Anl(u8 *data_buf,u8 num)
{
	u8 sum = 0;
	for(u8 i=0;i<(num-1);i++)
		sum += *(data_buf+i);
	if(!(sum==*(data_buf+num-1)))		return;		//�ж�sum
	if(!(*(data_buf)==0xAA && *(data_buf+1)==0xAF))		return;		//�ж�֡ͷ
	
	if(*(data_buf+2)==0X01)
	{
		if(*(data_buf+4)==0X01)
//			mpu6050.Acc_CALIBRATE = 1;
		if(*(data_buf+4)==0X02)
//			mpu6050.Gyro_CALIBRATE = 1;
		if(*(data_buf+4)==0X03)
		{
//			mpu6050.Acc_CALIBRATE = 1;		
//			mpu6050.Gyro_CALIBRATE = 1;			
		}
	}
	
	if(*(data_buf+2)==0X02)
	{
		if(*(data_buf+4)==0X01)
		{
			
	
			f.send_pid1 = 1;
			f.send_pid2 = 1;
//			f.send_pid3 = 1;

		}
		if(*(data_buf+4)==0X02)
		{
			
		}
		if(*(data_buf+4)==0XA0)		//��ȡ�汾��Ϣ
		{
			f.send_version = 1;
		}
		if(*(data_buf+4)==0XA1)		//�ָ�Ĭ�ϲ���
		{

		}
	}

	

	if(*(data_buf+2)==0X10)								//PID1
    {
		pidRateY.kp = 0.001*( (vs16)(*(data_buf+4)<<8)|*(data_buf+5) );
        pidRateY.ki = 0.001*( (vs16)(*(data_buf+6)<<8)|*(data_buf+7) );
        pidRateY.kd = 0.001*( (vs16)(*(data_buf+8)<<8)|*(data_buf+9) );
        pidRateX.kp = 0.001*( (vs16)(*(data_buf+10)<<8)|*(data_buf+11) );
        pidRateX.ki = 0.001*( (vs16)(*(data_buf+12)<<8)|*(data_buf+13) );
        pidRateX.kd = 0.001*( (vs16)(*(data_buf+14)<<8)|*(data_buf+15) );
        pidRateZ.kp = 0.001*( (vs16)(*(data_buf+16)<<8)|*(data_buf+17) );
        pidRateZ.ki = 0.001*( (vs16)(*(data_buf+18)<<8)|*(data_buf+19) );
        pidRateZ.kd = 0.001*( (vs16)(*(data_buf+20)<<8)|*(data_buf+21) );
        ANO_DT_Send_Check(*(data_buf+2),sum);
//		xSemaphoreGive(xBinarySemaphore);

    }
    if(*(data_buf+2)==0X11)								//PID2
    {
        pidRoll.kp = 0.001*( (vs16)(*(data_buf+4)<<8)|*(data_buf+5) );
        pidRoll.ki = 0.001*( (vs16)(*(data_buf+6)<<8)|*(data_buf+7) );
        pidRoll.kd = 0.001*( (vs16)(*(data_buf+8)<<8)|*(data_buf+9) );
        pidPitch.kp = 0.001*( (vs16)(*(data_buf+10)<<8)|*(data_buf+11) );
        pidPitch.ki = 0.001*( (vs16)(*(data_buf+12)<<8)|*(data_buf+13) );
        pidPitch.kd = 0.001*( (vs16)(*(data_buf+14)<<8)|*(data_buf+15) );
        pidYaw.kp = 0.001*( (vs16)(*(data_buf+16)<<8)|*(data_buf+17) );
        pidYaw.ki = 0.001*( (vs16)(*(data_buf+18)<<8)|*(data_buf+19) );
        pidYaw.kd = 0.001*( (vs16)(*(data_buf+20)<<8)|*(data_buf+21) );
			 
        ANO_DT_Send_Check(*(data_buf+2),sum);
//		xSemaphoreGive(xBinarySemaphore);

    }
    if(*(data_buf+2)==0X12)								//PID3
    {	

        ANO_DT_Send_Check(*(data_buf+2),sum);

    }
	if(*(data_buf+2)==0X13)								//PID4
	{
		ANO_DT_Send_Check(*(data_buf+2),sum);
	}
	if(*(data_buf+2)==0X14)								//PID5
	{
		ANO_DT_Send_Check(*(data_buf+2),sum);
	}
	if(*(data_buf+2)==0X15)								//PID6
	{
		ANO_DT_Send_Check(*(data_buf+2),sum);
	}
}

void ANO_DT_Send_Version(u8 hardware_type, u16 hardware_ver,u16 software_ver,u16 protocol_ver,u16 bootloader_ver)
{
	u8 _cnt=0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x00;
	data_to_send[_cnt++]=0;
	
	data_to_send[_cnt++]=hardware_type;
	data_to_send[_cnt++]=BYTE1(hardware_ver);
	data_to_send[_cnt++]=BYTE0(hardware_ver);
	data_to_send[_cnt++]=BYTE1(software_ver);
	data_to_send[_cnt++]=BYTE0(software_ver);
	data_to_send[_cnt++]=BYTE1(protocol_ver);
	data_to_send[_cnt++]=BYTE0(protocol_ver);
	data_to_send[_cnt++]=BYTE1(bootloader_ver);
	data_to_send[_cnt++]=BYTE0(bootloader_ver);
	
	data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++]=sum;
	
	USART1_DMA_Send(data_to_send, _cnt);
}
void ANO_DT_Send_Status(float angle_rol, float angle_pit, float angle_yaw, s32 alt, u8 fly_model, u8 armed)
{
	uint8_t cnt=0;
	vs16 temp;
	vs32 temp2;
	uint8_t sum = 0;
	uint8_t i;

	
	data_to_send[cnt++]=0xAA;		//֡ͷ��AAAA
	data_to_send[cnt++]=0xAA;		//
	data_to_send[cnt++]=0x01;		//�����֣�0xFnֻ�������ݣ�����ʾͼ��0x0n��ʾ���ݺ�ͼ��0x01��ʾ���͵���STATUS
	data_to_send[cnt++]=0;				//��Ҫ�������ݵ��ֽ������ݸ�0

	temp = (int)(angle_rol*100);				//�����
	data_to_send[cnt++]=BYTE1(temp);		//���ֽ�
	data_to_send[cnt++]=BYTE0(temp);		//���ֽ�
    
	temp = (int)(angle_pit*100);         //������    
	data_to_send[cnt++]=BYTE1(temp);
	data_to_send[cnt++]=BYTE0(temp);
    
	temp = (int)(angle_yaw*100);         //ƫ����
	data_to_send[cnt++]=BYTE1(temp);
	data_to_send[cnt++]=BYTE0(temp);

	temp2 = (int32_t)(0);   //�߶� 
	data_to_send[cnt++]=BYTE3(temp2);
	data_to_send[cnt++]=BYTE2(temp2);
	data_to_send[cnt++]=BYTE1(temp2);
	data_to_send[cnt++]=BYTE0(temp2);

	data_to_send[cnt++]=0x01;		//����ģʽ    01����̬  02������  03������
	data_to_send[cnt++]= 0;		//����״̬  0����   1����

	data_to_send[3] = cnt-4;		//�����ֽ���
	
	sum = 0;
	for(i=0;i<cnt;i++)		//����У��λ
		sum += data_to_send[i];
	data_to_send[cnt++]=sum;
	USART1_DMA_Send(data_to_send,cnt);
	
}

void ANO_DT_Send_Senser(s16 a_x,s16 a_y,s16 a_z,s16 g_x,s16 g_y,s16 g_z,s16 m_x,s16 m_y,s16 m_z)
{
	
	u8 _cnt=0;
	vs16 _temp;
	
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x02;
	data_to_send[_cnt++]=0;
	
	_temp = a_x;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = a_y;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = a_z;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	_temp = g_x;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = g_y;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = g_z;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	_temp = m_x;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = m_y;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = m_z;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++] = sum;
	
	USART1_DMA_Send(data_to_send, _cnt);
}

void ANO_DT_Send_RCData(u16 thr,u16 yaw,u16 rol,u16 pit,u16 aux1,u16 aux2,u16 aux3,u16 aux4,u16 aux5,u16 aux6)
{
	u8 _cnt=0;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x03;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=BYTE1(thr);
	data_to_send[_cnt++]=BYTE0(thr);
	data_to_send[_cnt++]=BYTE1(yaw);
	data_to_send[_cnt++]=BYTE0(yaw);
	data_to_send[_cnt++]=BYTE1(rol);
	data_to_send[_cnt++]=BYTE0(rol);
	data_to_send[_cnt++]=BYTE1(pit);
	data_to_send[_cnt++]=BYTE0(pit);
	data_to_send[_cnt++]=BYTE1(aux1);
	data_to_send[_cnt++]=BYTE0(aux1);
	data_to_send[_cnt++]=BYTE1(aux2);
	data_to_send[_cnt++]=BYTE0(aux2);
	data_to_send[_cnt++]=BYTE1(aux3);
	data_to_send[_cnt++]=BYTE0(aux3);
	data_to_send[_cnt++]=BYTE1(aux4);
	data_to_send[_cnt++]=BYTE0(aux4);
	data_to_send[_cnt++]=BYTE1(aux5);
	data_to_send[_cnt++]=BYTE0(aux5);
	data_to_send[_cnt++]=BYTE1(aux6);
	data_to_send[_cnt++]=BYTE0(aux6);

	data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	USART1_DMA_Send(data_to_send, _cnt);
}
void ANO_DT_Send_Power(u16 votage, u16 current)
{
	u8 _cnt=0;
	u16 temp;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x05;
	data_to_send[_cnt++]=0;
	
	temp = votage;
	data_to_send[_cnt++]=BYTE1(temp);
	data_to_send[_cnt++]=BYTE0(temp);
	temp = current;
	data_to_send[_cnt++]=BYTE1(temp);
	data_to_send[_cnt++]=BYTE0(temp);
	
	data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	USART1_DMA_Send(data_to_send, _cnt);
}
void ANO_DT_Send_MotoPWM(u16 m_1,u16 m_2,u16 m_3,u16 m_4,u16 m_5,u16 m_6,u16 m_7,u16 m_8)
{
	u8 _cnt=0;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x06;
	data_to_send[_cnt++]=0;
	
	data_to_send[_cnt++]=BYTE1(m_1);
	data_to_send[_cnt++]=BYTE0(m_1);
	data_to_send[_cnt++]=BYTE1(m_2);
	data_to_send[_cnt++]=BYTE0(m_2);
	data_to_send[_cnt++]=BYTE1(m_3);
	data_to_send[_cnt++]=BYTE0(m_3);
	data_to_send[_cnt++]=BYTE1(m_4);
	data_to_send[_cnt++]=BYTE0(m_4);
	data_to_send[_cnt++]=BYTE1(m_5);
	data_to_send[_cnt++]=BYTE0(m_5);
	data_to_send[_cnt++]=BYTE1(m_6);
	data_to_send[_cnt++]=BYTE0(m_6);
	data_to_send[_cnt++]=BYTE1(m_7);
	data_to_send[_cnt++]=BYTE0(m_7);
	data_to_send[_cnt++]=BYTE1(m_8);
	data_to_send[_cnt++]=BYTE0(m_8);
	
	data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	USART1_DMA_Send(data_to_send, _cnt);
}


void ANO_DT_Send_PID(u8 group,float p1_p,float p1_i,float p1_d,float p2_p,float p2_i,float p2_d,float p3_p,float p3_i,float p3_d)
{
	u8 _cnt=0;
	vs16 _temp;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x10+group-1;
	data_to_send[_cnt++]=0;
	
	
	_temp = p1_p * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p1_i  * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p1_d  * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p2_p  * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p2_i  * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p2_d * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p3_p  * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p3_i  * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p3_d * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;

	USART1_DMA_Send(data_to_send, _cnt);
}


