#include "sys.h" // Device header

TaskHandle_t Task1_Handler;
TaskHandle_t Task2_Handler;
TaskHandle_t Task3_Handler;
TaskHandle_t AppTaskCreate_Handle;
BaseType_t xReturn;
SemaphoreHandle_t xBinarySemaphore;


void Task1(void *pvParameters);
void Task2(void *pvParameters);
void Task3(void *pvParameters);
void HardWare_Init(void);

uint16_t count = 0;
bool SendStatus = 0;

bool Update_Lock=0;

extern MOTOR_OUT Motor_Out;

extern uint8_t data_to_send[50];	//发送数据缓存

//typedef struct
//{
//	int16_t ROLL;
//	int16_t PITCH;
//	int16_t THROTTLE;
//	int16_t YAW;
//}RC_TYPE;





void SendToRemote(void)
{
	int16_t temp; 	
	TxBuf[0] = 0xFF;//帧头
	if(FLY_UnLock)
	{
		TxBuf[1] = 1;
	}
	else
	{
		TxBuf[1] = 0;
	}

	temp = (u16)RC.THROTTLE; //油门
	TxBuf[2] = Byte1(temp);
	TxBuf[3] = Byte0(temp);
	temp = (int)(Angle.yaw*100); //航向
	TxBuf[4] = Byte1(temp);
	TxBuf[5] = Byte0(temp);
	temp = (int)(Angle.pitch*100); //俯仰
	TxBuf[6] = Byte1(temp);
	TxBuf[7] = Byte0(temp);
	temp = (int)(Angle.roll*100); //横滚
	TxBuf[8] = Byte1(temp);
	TxBuf[9] = Byte0(temp);
	temp = (int)(BAT.BattMeasureV*100); //飞机电池电压
	TxBuf[12] = Byte1(temp);
	TxBuf[13] = Byte0(temp);
	SI24R1_SendPacket(TxBuf); //SI24R1发送函数
}



void TIM4_IRQHandler(void)
{

	
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
		// BaseType_t xHigherPriorityTaskWoken;
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);

		if(Update_Lock == 0)
		{
			count++;
			if (count == 3)
			{
				count=0;
				Update_Lock=1;
				
			}
			
		}
		
		
		
//			//	  /* 进入临界段，临界段可以嵌套 */
//			taskENTER_CRITICAL_FROM_ISR();
//			//20ms读取一次mpu数据
//			
//			// 给信号量发送一个信号
//	
//		
//				if (xSemaphoreGiveFromISR(xBinarySemaphore,&xHigherPriorityTaskWoken) != pdPASS)
//				{
//					// 处理错误，信号量操作失败
////					OLED_ShowString(1,1,"Fail");
//					
//				}else
//				{
////					OLED_ShowString(1,1,"ok");
//				}
//				count = 0;
//			
//			// 如果唤醒了更高优先级的任务，设置相应的标志
//			

//		
//			taskENTER_CRITICAL_FROM_ISR();
//		}
	
    }
}

extern RC_TYPE RC;

void Task1(void *pvParameters)
{


	TickType_t xLastWakeTime = xTaskGetTickCount(); // 获取当前的系统时间
	for (;;)
	{
	    BATT_GetVoltage();
		MpuGetData();
		GetAngle(&MPU6050,&Angle,0.00626f);
//		FlightPidControl(0.003f);
//		MotorControl();
		if(FLYDataRx_OK)
		{
			FLYDataRx_OK= 0 ;
			SendToRemote();
		}
		vTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS(10)); // 20毫秒后再
	}
}





void Task2(void *pvParameters)
{
	TickType_t xLastWakeTime = xTaskGetTickCount(); // 获取当前的系统时间
	
	for (;;)
	{
		vTaskDelayUntil( &xLastWakeTime,  pdMS_TO_TICKS(50) );	
	}
}


void Task3(void *pvParameters)
{
	TickType_t xLastWakeTime = xTaskGetTickCount(); // 获取当前的系统时间
	for (;;)
	{
		ANO_DT_Data_Exchange();
		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1)); // 
	}
}

static void AppTaskCreate(void)
{
	taskENTER_CRITICAL(); // 进入临界区

	xReturn = xTaskCreate((TaskFunction_t)Task1,		   /* 任务入口函数 */
						  (const char *)"Task1",		   /* 任务名字 */
						  (uint16_t)256,				   /* 任务栈大小 */
						  (void *)NULL,					   /* 任务入口函数参数 */
						  (UBaseType_t)3,				   /* 任务的优先级 */
						  (TaskHandle_t *)&Task1_Handler); /* 任务控制块指针 */
	if(pdPASS != xReturn)
	{
		return;
	}
								  
	// xReturn = xTaskCreate((TaskFunction_t)Task2,		   /* 任务入口函数 */
	// 				  (const char *)"Task2",		   /* 任务名字 */
	// 				  (uint16_t)256,				   /* 任务栈大小 */
	// 				  (void *)NULL,					   /* 任务入口函数参数 */
	// 				  (UBaseType_t)3,				   /* 任务的优先级 */
	// 				  (TaskHandle_t *)&Task2_Handler); /* 任务控制块指针 */		
	// if(pdPASS != xReturn)
	// {
	// 	return;
	// }

	xReturn = xTaskCreate((TaskFunction_t)Task3,		   /* 任务入口函数 */
					  (const char *)"Task3",		   /* 任务名字 */
					  (uint16_t)256,				   /* 任务栈大小 */
					  (void *)NULL,					   /* 任务入口函数参数 */
					  (UBaseType_t)3,				   /* 任务的优先级 */
					  (TaskHandle_t *)&Task3_Handler); /* 任务控制块指针 */	

	if(pdPASS != xReturn)
	{
		return;
	}					
	vTaskDelete(AppTaskCreate_Handle); // 删除AppTaskCreate任务
	taskEXIT_CRITICAL();			   // 退出临界区
}

void HardWare_Init(void)
{
	delay_init();
	NVIC_init();
	Serial_Init();
	Led_Init();
	Battery_Init();
	USART1_DMA_Init(DMA1_Channel4, (uint32_t)&USART1->DR, (uint32_t *)data_to_send, 50);
	// Timer4_Init();
	#ifdef AXIS_USE_OLED
		OLED_Init();
	#endif
	IIC_Init();		// 初始化IIC
	PID_ParamInit();
	MpuInit();	
	SI24R1_Config();
	TIM3_PWM_Init();

}

int main(void)
{

	HardWare_Init(); //硬件初始化

	xBinarySemaphore = xSemaphoreCreateBinary();
	if (xBinarySemaphore)
	{
			/* 创建AppTaskCreate任务 */
		xReturn = xTaskCreate((TaskFunction_t)AppTaskCreate,		  /* 任务入口函数 */
							(const char *)"AppTaskCreate",		  /* 任务名字 */
							(uint16_t)512,						  /* 任务栈大小 */
							(void *)NULL,							  /* 任务入口函数参数 */
							(UBaseType_t)1,						  /* 任务的优先级 */
							(TaskHandle_t *)&AppTaskCreate_Handle); /* 任务控制块指针 */

		/* 启动任务调度 */
		if (pdPASS == xReturn)
		{
			vTaskStartScheduler(); /* 启动任务，开启调度 */
		}
		else
			return -1;
	}
}
