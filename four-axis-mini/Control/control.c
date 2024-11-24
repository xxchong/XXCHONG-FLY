#include "sys.h"

PidObject pidRateX; //�ڻ�PID����
PidObject pidRateY;
PidObject pidRateZ;

PidObject pidPitch; //�⻷PID����
PidObject pidRoll;
PidObject pidYaw;

PidObject pidHeightRate;
PidObject pidHeightHigh;

extern RC_TYPE RC;
MOTOR_OUT Motor_Out;//������


PidObject *(pPidObject[])={&pidRateX,&pidRateY,&pidRateZ,&pidRoll,&pidPitch,&pidYaw   //�ṹ�����飬��ÿһ�������һ��pid�ṹ�壬�����Ϳ���������������PID��������  �������ʱ������λpid�������ݣ�����������仰�����þͿ�����
		,&pidHeightRate
		,&pidHeightHigh
};


/******************************************************************************************

                     ��ͷ(Y+)

                  M4    ��    M3
                    \   |   /
                     \  |  /
                      \ | /
                ����������������+����������������>X+
                      / | \
                     /  |  \
                    /   |   \
                  M1    |    M2


    1. M1 M3�����ʱ����ת��M2 M4���˳ʱ����ת
    2. X:��MPU6050�� X �ᣬY:��MPU6050�� Y �ᣬZ��������ֱ X-Y �棬��ֱ����
	3. �� X ����תΪPITCH �� 
	   �� Y ����תΪ ROLL �� 
	   �� Z ����תΪ YAW  ��
*******************************************************************************************/
void FlightPidControl(float dt)
{
	volatile static uint8_t status=WAITING_1;
	status = PROCESS_31;	
	
	pidPitch.desired= 0;	 //��ң��ֵ��Ϊ���нǶȵ�����ֵ
	pidRoll.desired	= 0;
	pidYaw.desired = 0;

	
	switch(status)
	{		
		case WAITING_1: //�ȴ�����
						if(ALL_flag.unlock)
						{
							status = READY_11;	
						}			
						break;
		case READY_11:  //׼���������
						{
							pidRest(pPidObject,8); //������λPID���ݣ���ֹ�ϴ�����������Ӱ�챾�ο���
							Angle.yaw = pidYaw.desired =  pidYaw.measured = 0;   //����ƫ����
							status = PROCESS_31;
						}
						break;			
		case PROCESS_31: //��ʽ�������
						{
							pidRateX.measured = MPU6050.gyroX * Gyro_G; //�ڻ�����ֵ �Ƕ�/��
							pidRateY.measured = MPU6050.gyroY * Gyro_G;
							pidRateZ.measured = MPU6050.gyroZ * Gyro_G;

							pidPitch.measured = Angle.pitch; //�⻷����ֵ ��λ���Ƕ�
							pidRoll.measured = Angle.roll;
							pidYaw.measured = Angle.yaw;

							pidUpdate(&pidRoll,dt);    //����PID���������������⻷	�����PID		
							pidRateX.desired = pidRoll.out; //���⻷��PID�����Ϊ�ڻ�PID������ֵ��Ϊ����PID
							pidUpdate(&pidRateX,dt);  //�ٵ����ڻ�

							pidUpdate(&pidPitch,dt);    //����PID���������������⻷	������PID	
							pidRateY.desired = pidPitch.out;  
							pidUpdate(&pidRateY,dt); //�ٵ����ڻ�

							CascadePID(&pidRateZ,&pidYaw,dt);	//Ҳ����ֱ�ӵ��ô���PID����������
						}
						break;
		case EXIT_255:  //�˳�����
						{
							pidRest(pPidObject,8);
							status = WAITING_1;//���صȴ�����
						}
						break;
		default:	status = EXIT_255;break;
	}
	//�����������ʹ��ң�ؽ����������ɿؾͿ������κ�����½�����ֹ���У��������������˳�PID����
	if(ALL_flag.unlock == EMERGENT) status = EXIT_255;
}
void MotorControl(void)
{	


//	status = PROCESS_31;	
//		Moto_PWM_1 = rc_in->THROTTLE + PID_ROL_Rate.OutPut + PID_PIT_Rate.OutPut - PID_YAW_Rate.OutPut; 
//		Moto_PWM_2 = rc_in->THROTTLE - PID_ROL_Rate.OutPut + PID_PIT_Rate.OutPut + PID_YAW_Rate.OutPut;  
//		Moto_PWM_3 = rc_in->THROTTLE - PID_ROL_Rate.OutPut - PID_PIT_Rate.OutPut - PID_YAW_Rate.OutPut;   
//		Moto_PWM_4 = rc_in->THROTTLE + PID_ROL_Rate.OutPut - PID_PIT_Rate.OutPut + PID_YAW_Rate.OutPut;
//	

//	if(RC.THROTTLE > 100 && RC.THROTTLE_LOCK)
//	{
//		Motor_Out.M1 = rc_in->THROTTLE   + pidRateY.out + pidRateX.out + pidRateZ.out;//; ��̬����������������Ŀ�����
//		Motor_Out.M2 = rc_in->THROTTLE   - pidRateY.out + pidRateX.out - pidRateZ.out ;//
//		Motor_Out.M3 = rc_in->THROTTLE   - pidRateY.out - pidRateX.out + pidRateZ.out;
//		Motor_Out.M4 = rc_in->THROTTLE   + pidRateY.out - pidRateX.out - pidRateZ.out;//;
//		
//		
////		Motor_Out.M1 = rc_in->THROTTLE   + pidRateX.out + pidRateY.out + pidRateZ.out;//; ��̬����������������Ŀ�����
////		Motor_Out.M2 = rc_in->THROTTLE   + pidRateX.out - pidRateY.out - pidRateZ.out ;//
////		Motor_Out.M3 = rc_in->THROTTLE   - pidRateX.out + pidRateY.out + pidRateZ.out;
////		Motor_Out.M4 = rc_in->THROTTLE  - pidRateX.out - pidRateY.out - pidRateZ.out;//;
//		
//		Motor_Pwm_Limit(&Motor_Out.M1,&Motor_Out.M2,&Motor_Out.M3,&Motor_Out.M4);
//		Motor_Set_Pwm(Motor_Out.M1,Motor_Out.M2,Motor_Out.M3,Motor_Out.M4);
//	
//		
//	}else
//	{
//		Motor_Set_Pwm(0,0,0,0);
//	}
	

	

	
	
} 

void PID_ParamInit(void)
{
	pidRateX.kp = 3.0f;
	pidRateY.kp = 0.0f;
	pidRateZ.kp = 0.0f;

	pidRateX.ki = 0.0f;
	pidRateY.ki = 0.0f;
	pidRateZ.ki = 0.0f;	

	pidRateX.kd = 0.0f;
	pidRateY.kd = 0.0f;
	pidRateZ.kd = 0.0f;	

	pidPitch.kp	= 3.0f;
	pidRoll.kp	= 0.0f;
	pidYaw.kp	= 0.0f;	
	
	pidPitch.ki	= 0.0f;
	pidRoll.ki	= 0.0f;
	pidYaw.ki	= 0.0f;	
	
	pidPitch.kd	= 0.0f;
	pidRoll.kd  = 0.0f;
	pidYaw.kd	= 0.0f;	
	
	
	
	
	
	
//	
//		if( RC.THROTTLE > 100 &&  RC.THROTTLE_LOCK == 1)
//	{
//			
//		Motor_Out.M1 = rc_in->THROTTLE   + pidRateX.out + pidRateY.out + pidRateZ.out;//; ��̬����������������Ŀ�����
//		Motor_Out.M2 = rc_in->THROTTLE   + pidRateX.out - pidRateY.out - pidRateZ.out ;//
//		Motor_Out.M3 = rc_in->THROTTLE   - pidRateX.out + pidRateY.out + pidRateZ.out;
//		Motor_Out.M4 = rc_in->THROTTLE  - pidRateX.out - pidRateY.out - pidRateZ.out;//;
//		
//		Motor_Pwm_Limit(&Motor_Out.M1,&Motor_Out.M2,&Motor_Out.M3,&Motor_Out.M4);
//		Motor_Set_Pwm(Motor_Out.M1,Motor_Out.M2,Motor_Out.M3,Motor_Out.M4);
//	}else
//	{
//		
//		Motor_Set_Pwm(0,0,0,0);
//	}

	
	
	
	
	
	
	
	
	
	
}
