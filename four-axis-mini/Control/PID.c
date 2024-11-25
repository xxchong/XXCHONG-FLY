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
	error = pid->desired - pid->measured; //当前角度与实际角度的误差
	pid->integ += error * dt;	 //误差积分累加值
	//  pid->integ = LIMIT(pid->integ,pid->IntegLimitLow,pid->IntegLimitHigh); //进行积分限幅
	deriv = (error - pid->prevError)/dt;  //前后两次误差做微分
	pid->out = pid->kp * error + pid->ki * pid->integ + pid->kd * deriv;//PID输出
	//pid->out = LIMIT(pid->out,pid->OutLimitLow,pid->OutLimitHigh); //输出限幅
	pid->prevError = error;  //更新上次的误差	
}

void CascadePID(PidObject* pidRate,PidObject* pidAngE,const float dt)  //串级PID
{	 
	pidUpdate(pidAngE,dt);    //先计算外环
	pidRate->desired = pidAngE->out;
	pidUpdate(pidRate,dt);    //再计算内环	
}