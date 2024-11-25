#include "SYS.h"

TaskHandle_t AppTaskCreate_Handle;
TaskHandle_t Task1_Handler;
TaskHandle_t Task2_Handler;
TaskHandle_t Task3_Handler;
SemaphoreHandle_t SemapMTX_Handle_t;
SemaphoreHandle_t SemapMRX_Handle_t;
SemaphoreHandle_t xBinarySemaphore;
QueueHandle_t Queue1;

u8g2_t u8g2;
TimerHandle_t xAutoReloadTimer1;
TimerHandle_t xAutoReloadTimer2;
char LeftX[20];
char RightY[20];
char RightX[20];
char LeftY[20];




extern u8 packetData[12];
extern uint16_t j;
extern uint16_t Count_DMA;



extern KEY Keys[4];
extern bool Tx_OK;
extern bool Rx_OK;
int SendNum=0;


void Key_Proc(void)
{
	
	if(Keys[0].Key_Flag_1 == 1)
	{
		
		FLY.Lock=!FLY.Lock;
		Keys[0].Key_Flag_1 = 0;
		
	}
	
}

		
	
char OLED_Buf[30];

/*遥控器发送任务*/
void Task1(void *pvParameters)
{
u8g2Init(&u8g2);
	u8g2_SetFont(&u8g2, u8g2_font_6x10_tf); // 字体
	u8g2_ClearBuffer(&u8g2);

	Calibration_UI();
	u8g2_ClearBuffer(&u8g2);
	while (1)
	{
		
		if(Page == 0)
		{
			
			MAIN_UI();
		}else if(Page == 1)
		{
			Second_Page();
			
		}
		




	
	
	

	}
}

/*遥控器接收处理任务*/
void Task2(void *pvParameters)
{
	while (1)
	{


	}
			
			


}


static void AppTaskCreate(void)
{
	BaseType_t xReturn = pdPASS; /* 定义一个创建信息返回值，默认为pdPASS */

	taskENTER_CRITICAL(); // 进入临界区

	xReturn = xTaskCreate((TaskFunction_t)Task1,		   /* 任务入口函数 */
						  (const char *)"Task1",		   /* 任务名字 */
						  (uint16_t)256,				   /* 任务栈大小 */
						  (void *)NULL,					   /* 任务入口函数参数 */
						  (UBaseType_t)3,				   /* 任务的优先级 */
						  (TaskHandle_t *)&Task1_Handler); /* 任务控制块指针 */
						  
//	xReturn = xTaskCreate((TaskFunction_t)Task2,		   /* 任务入口函数 */
//						  (const char *)"Task2",		   /* 任务名字 */
//						  (uint16_t)256,				   /* 任务栈大小 */
//						  (void *)NULL,					   /* 任务入口函数参数 */
//						  (UBaseType_t)3,				   /* 任务的优先级 */
//						  (TaskHandle_t *)&Task2_Handler); /* 任务控制块指针 */
//						  					  
//						
	vTaskDelete(AppTaskCreate_Handle); /*删除AppTaskCreate任务*/
	taskEXIT_CRITICAL();			   /*退出临界区*/
}

void BSP_Init(void)
{
	Delay_init();
	NVIC_init();
	IIC_Init();
	KEY_Init();
	LED_Init();
	TIM_OCTigrConfig();
	SI24R1_Config();
//	USART1_Config(115200);
	ADC_PS2_Init();	/*ADC1 DMA转运*/

}

int main(void)
{
	BSP_Init();
	xBinarySemaphore = xSemaphoreCreateBinary();
	BaseType_t xReturn = pdPASS; /* 定义一个创建信息返回值，默认为pdPASS */

	/* 创建AppTaskCreate任务 */
	xReturn = xTaskCreate((TaskFunction_t)AppTaskCreate,		  /* 任务入口函数 */
						  (const char *)"AppTaskCreate",		  /* 任务名字 */
						  (uint16_t)512,						  /* 任务栈大小 */
						  (void *)NULL,							  /* 任务入口函数参数 */
						  (UBaseType_t)3,						  /* 任务的优先级 */
						  (TaskHandle_t *)&AppTaskCreate_Handle); /* 任务控制块指针 */
	/* 启动任务调度 */
	if (pdPASS == xReturn)
	{
		
//		OpenShow();
		
	
		
		vTaskStartScheduler(); /* 启动任务，开启调度 */
	}
	else
		return -1;
}
