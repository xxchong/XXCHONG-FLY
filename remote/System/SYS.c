#include "SYS.h"                  // Device header
#include "Image.h" 

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


// void EXTI15_10_IRQHandler(void)
// {

// 	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
// 	if (EXTI_GetITStatus(EXTI_Line12) != RESET)
// 	{
// 		printf("EXTI0_IRQHandler\r\n ");
// 		/* 使用定时器消除抖动 */
// //		xTimerResetFromISR(xAutoReloadTimer1, &xHigherPriorityTaskWoken); /* Tcur + 2000 */
// 		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
// 		EXTI_ClearITPendingBit(EXTI_Line12); // 清除中断
// 	}
// }


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