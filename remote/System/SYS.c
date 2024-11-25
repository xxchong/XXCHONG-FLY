#include "SYS.h"                  // Device header
#include "Image.h" 



//中断优先级组
#define NVIC_PRIORITY_GROUP_3          3 /* Preemption: 4 bits / Subpriority: 0 bits                        */
#define NVIC_PRIORITY_GROUP_4          4 /* Preemption: 3 bits / Subpriority: 1 bits                        */
#define NVIC_PRIORITY_GROUP_5          5 /* Preemption: 2 bits / Subpriority: 2 bits                        */
#define NVIC_PRIORITY_GROUP_6          6 /* Preemption: 1 bits / Subpriority: 3 bits                        */
#define NVIC_PRIORITY_GROUP_7          7 /* Preemption: 0 bits / Subpriority: 4 bits                        */


//为所有用到的中断配置抢占优先级和响应优先级
//void NVIC_Config(void)
//{

//	
//	//SysTick_IRQn中断由于在有些中断函数中也会使用到，所以它的优先级是最高的
////	NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_PRIORITY_GROUP_5, 0, 0)); 			//SysTick_IRQn中断
//	NVIC_SetPriority(EXTI9_5_IRQn, NVIC_EncodePriority(NVIC_PRIORITY_GROUP_5, 1, 0));				//EXTI1_IRQn中断，SI24R1的IRQ引脚中断
//	NVIC_SetPriority(DMA1_Channel1_IRQn, NVIC_EncodePriority(NVIC_PRIORITY_GROUP_5, 1, 1));		//DMA1_Channel1_IRQn中断，DMA将ADC数据传输完成中断
////	NVIC_SetPriority(EXTI15_10_IRQn, NVIC_EncodePriority(NVIC_PRIORITY_GROUP_5, 2, 2));			//EXTI15_10_IRQn中断,按键触发中断（优先级必须小于EXTI1_IRQn否则会导致SI24R1l连接中断）
//}


void NVIC_init(void)
{
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设置中断优先级分组  
  
    // 外部中断配置为最高优先级  
    NVIC_InitTypeDef NVIC_InitStructure;  
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn; // 假设外部中断是EXTI0  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; // 抢占优先级设置为0（最高）  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00; // 子优先级任意设置  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // 使能中断  
    NVIC_Init(&NVIC_InitStructure); // 初始化NVIC  
  
    // DMA中断配置为次高优先级  
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn; // 假设DMA中断是DMA1 Channel 1  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; // 抢占优先级设置为1（次高）  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01; // 子优先级任意设置  
    NVIC_Init(&NVIC_InitStructure); // 初始化NVIC  
  
//    // 定时器中断配置为最低优先级  
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; // 假设定时器中断是TIM2  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; // 抢占优先级设置为15（最低）  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01; // 子优先级任意设置  
    NVIC_Init(&NVIC_InitStructure); // 初始化NVIC  
	
	
//    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; // 假设定时器中断是TIM2  
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; // 抢占优先级设置为15（最低）  
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01; // 子优先级任意设置  
//    NVIC_Init(&NVIC_InitStructure); // 初始化NVIC  
	
	
	
	

}








//void EXTI15_10_IRQHandler(void)
//{

//	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
//	if (EXTI_GetITStatus(EXTI_Line12) != RESET)
//	{
//		printf("EXTI0_IRQHandler\r\n ");
//		/* 使用定时器消除抖动 */
////		xTimerResetFromISR(xAutoReloadTimer1, &xHigherPriorityTaskWoken); /* Tcur + 2000 */
//		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
//		EXTI_ClearITPendingBit(EXTI_Line12); // 清除中断
//	}
//}
void ShowStackSt(void)
{
	//		memset(pcWriteBuffer,0,400);				//信息缓冲区清零

	//    vTaskList((char *)pcWriteBuffer);  //获取任务运行时间信息

	//   printf("---------------------------------------------\r\n");
	//   printf("任务名      任务状态 优先级   剩余栈 任务序号\r\n");
	//   printf("%s", pcWriteBuffer);
	//   printf("---------------------------------------------\r\n");

	//   memset(pcWriteBuffer,0,400);				//信息缓冲区清零

	//   vTaskGetRunTimeStats((char *)pcWriteBuffer);

	//   printf("任务名       运行计数         使用率\r\n");
	//   printf("%s", pcWriteBuffer);
	//   printf("---------------------------------------------\r\n\n");
	//
}

void MyStr(char *STR,char *Return) 
{
    char* start_ptr;
    char* end_ptr;

    // 查找子字符串的起始位置
    start_ptr = strchr((char *)STR, '{');
    if (start_ptr == NULL) {
        printf("Find First Fail‘{’\r\n");
    }
    // 查找子字符串的结束位置
    end_ptr = strchr((char *)start_ptr + 1, '}');
    if (end_ptr == NULL) {
        printf("Find First Fail‘}’\r\n");
    }
 
    // 复制子字符串到 sub_str
    strncpy((char *)Return, start_ptr, end_ptr - start_ptr + 1);
    Return[end_ptr - start_ptr + 1] = '\0';
    printf("Get String:%s\r\n", Return);

}
//void OpenShow(void)
//{

//	u8g2Init(&u8g2);
//	
//	for (uint8_t i = 0; i <= 100; i = i + 10)
//	{
//		Open_Show_Led();
//		u8g2_SetFont(&u8g2, u8g2_font_6x10_tf); // 字体
//		u8g2_DrawBox(&u8g2, 0, 59, i, 4);	   // 填充框实心矩形框
//		u8g2_DrawFrame(&u8g2, 0,58, 128, 6); // 空心矩形框
//		
//		u8g2_DrawXBM(&u8g2,2,0,124,58,(const uint8_t *)gImage_Balance_Car_Img);

//		
//		
//		if (i == 100)
//		{
//			u8g2_ClearBuffer(&u8g2);
//			u8g2_DrawUTF8(&u8g2, 20, 36, "Hardware Init Ok");
//			GPIO_SetBits(GPIOB, GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
//		} 
//		u8g2_SendBuffer(&u8g2);
//	}
//	
//	
//	
//}


