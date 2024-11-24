#include "sys.h" 



void pidRest(PidObject **pid,const uint8_t len)
{
	uint8_t i;
	for(i=0;i<len;i++)
	{
	  	pid[i]->integ = 0;
	    pid[i]->prevError = 0;
	    pid[i]->out = 0;
		pid[i]->offset = 0;
	}
}


void pidUpdate(PidObject* pid,const float dt)
{
	float error;
	float deriv;
	error = pid->desired - pid->measured; //��ǰ�Ƕ���ʵ�ʽǶȵ����
	pid->integ += error * dt;	 //�������ۼ�ֵ
	//  pid->integ = LIMIT(pid->integ,pid->IntegLimitLow,pid->IntegLimitHigh); //���л����޷�
	deriv = (error - pid->prevError)/dt;  //ǰ�����������΢��
	pid->out = pid->kp * error + pid->ki * pid->integ + pid->kd * deriv;//PID���
	//pid->out = LIMIT(pid->out,pid->OutLimitLow,pid->OutLimitHigh); //����޷�
	pid->prevError = error;  //�����ϴε����	
}

void CascadePID(PidObject* pidRate,PidObject* pidAngE,const float dt)  //����PID
{	 
	pidUpdate(pidAngE,dt);    //�ȼ����⻷
	pidRate->desired = pidAngE->out;
	pidUpdate(pidRate,dt);    //�ټ����ڻ�	
}






