#include "SYS.h"                  // Device header
#include "Image.h" 



//�ж����ȼ���
#define NVIC_PRIORITY_GROUP_3          3 /* Preemption: 4 bits / Subpriority: 0 bits                        */
#define NVIC_PRIORITY_GROUP_4          4 /* Preemption: 3 bits / Subpriority: 1 bits                        */
#define NVIC_PRIORITY_GROUP_5          5 /* Preemption: 2 bits / Subpriority: 2 bits                        */
#define NVIC_PRIORITY_GROUP_6          6 /* Preemption: 1 bits / Subpriority: 3 bits                        */
#define NVIC_PRIORITY_GROUP_7          7 /* Preemption: 0 bits / Subpriority: 4 bits                        */


//Ϊ�����õ����ж�������ռ���ȼ�����Ӧ���ȼ�
//void NVIC_Config(void)
//{

//	
//	//SysTick_IRQn�ж���������Щ�жϺ�����Ҳ��ʹ�õ��������������ȼ�����ߵ�
////	NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_PRIORITY_GROUP_5, 0, 0)); 			//SysTick_IRQn�ж�
//	NVIC_SetPriority(EXTI9_5_IRQn, NVIC_EncodePriority(NVIC_PRIORITY_GROUP_5, 1, 0));				//EXTI1_IRQn�жϣ�SI24R1��IRQ�����ж�
//	NVIC_SetPriority(DMA1_Channel1_IRQn, NVIC_EncodePriority(NVIC_PRIORITY_GROUP_5, 1, 1));		//DMA1_Channel1_IRQn�жϣ�DMA��ADC���ݴ�������ж�
////	NVIC_SetPriority(EXTI15_10_IRQn, NVIC_EncodePriority(NVIC_PRIORITY_GROUP_5, 2, 2));			//EXTI15_10_IRQn�ж�,���������жϣ����ȼ�����С��EXTI1_IRQn����ᵼ��SI24R1l�����жϣ�
//}


void NVIC_init(void)
{
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // �����ж����ȼ�����  
  
    // �ⲿ�ж�����Ϊ������ȼ�  
    NVIC_InitTypeDef NVIC_InitStructure;  
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn; // �����ⲿ�ж���EXTI0  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; // ��ռ���ȼ�����Ϊ0����ߣ�  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00; // �����ȼ���������  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // ʹ���ж�  
    NVIC_Init(&NVIC_InitStructure); // ��ʼ��NVIC  
  
    // DMA�ж�����Ϊ�θ����ȼ�  
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn; // ����DMA�ж���DMA1 Channel 1  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; // ��ռ���ȼ�����Ϊ1���θߣ�  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01; // �����ȼ���������  
    NVIC_Init(&NVIC_InitStructure); // ��ʼ��NVIC  
  
//    // ��ʱ���ж�����Ϊ������ȼ�  
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; // ���趨ʱ���ж���TIM2  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; // ��ռ���ȼ�����Ϊ15����ͣ�  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01; // �����ȼ���������  
    NVIC_Init(&NVIC_InitStructure); // ��ʼ��NVIC  
	
	
//    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; // ���趨ʱ���ж���TIM2  
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; // ��ռ���ȼ�����Ϊ15����ͣ�  
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01; // �����ȼ���������  
//    NVIC_Init(&NVIC_InitStructure); // ��ʼ��NVIC  
	
	
	
	

}








//void EXTI15_10_IRQHandler(void)
//{

//	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
//	if (EXTI_GetITStatus(EXTI_Line12) != RESET)
//	{
//		printf("EXTI0_IRQHandler\r\n ");
//		/* ʹ�ö�ʱ���������� */
////		xTimerResetFromISR(xAutoReloadTimer1, &xHigherPriorityTaskWoken); /* Tcur + 2000 */
//		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
//		EXTI_ClearITPendingBit(EXTI_Line12); // ����ж�
//	}
//}
void ShowStackSt(void)
{
	//		memset(pcWriteBuffer,0,400);				//��Ϣ����������

	//    vTaskList((char *)pcWriteBuffer);  //��ȡ��������ʱ����Ϣ

	//   printf("---------------------------------------------\r\n");
	//   printf("������      ����״̬ ���ȼ�   ʣ��ջ �������\r\n");
	//   printf("%s", pcWriteBuffer);
	//   printf("---------------------------------------------\r\n");

	//   memset(pcWriteBuffer,0,400);				//��Ϣ����������

	//   vTaskGetRunTimeStats((char *)pcWriteBuffer);

	//   printf("������       ���м���         ʹ����\r\n");
	//   printf("%s", pcWriteBuffer);
	//   printf("---------------------------------------------\r\n\n");
	//
}

void MyStr(char *STR,char *Return) 
{
    char* start_ptr;
    char* end_ptr;

    // �������ַ�������ʼλ��
    start_ptr = strchr((char *)STR, '{');
    if (start_ptr == NULL) {
        printf("Find First Fail��{��\r\n");
    }
    // �������ַ����Ľ���λ��
    end_ptr = strchr((char *)start_ptr + 1, '}');
    if (end_ptr == NULL) {
        printf("Find First Fail��}��\r\n");
    }
 
    // �������ַ����� sub_str
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
//		u8g2_SetFont(&u8g2, u8g2_font_6x10_tf); // ����
//		u8g2_DrawBox(&u8g2, 0, 59, i, 4);	   // ����ʵ�ľ��ο�
//		u8g2_DrawFrame(&u8g2, 0,58, 128, 6); // ���ľ��ο�
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


