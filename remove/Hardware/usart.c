#include "SYS.h"

/*************************************************
*�������ƣ�USART1_Config
*����������void
*��������ֵ��void
*�������ܣ�	USART1 TX---PA9		����--->�����������
			USART1 RX----PA10	����--->��������
**************************************************/


void USART1_Config(uint32_t Baud)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = Baud;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1,ENABLE);

}


void USART2_Config(uint32_t Baud)
{
	GPIO_InitTypeDef GPIO_InitStrue;
	USART_InitTypeDef USART_InitStrue;

	// ����ʹ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

	
	// ��ʼ�� ���ڶ�ӦIO��  TX-PA2  RX-PA3
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStrue);
	
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_3;
	GPIO_Init(GPIOA,&GPIO_InitStrue);
	
	// ��ʼ�� ����ģʽ״̬
	USART_InitStrue.USART_BaudRate=Baud; // ������
	USART_InitStrue.USART_HardwareFlowControl=USART_HardwareFlowControl_None; // Ӳ��������
	USART_InitStrue.USART_Mode=USART_Mode_Tx|USART_Mode_Rx; // ���� ���� ģʽ��ʹ��
	USART_InitStrue.USART_Parity=USART_Parity_No; // û����żУ��
	USART_InitStrue.USART_StopBits=USART_StopBits_1; // һλֹͣλ
	USART_InitStrue.USART_WordLength=USART_WordLength_8b; // ÿ�η������ݿ��Ϊ8λ
	USART_Init(USART2,&USART_InitStrue);
	USART_Cmd(USART2, ENABLE);
	 // ʹ�ܴ��ڿ����ж�
    NVIC_EnableIRQ(USART2_IRQn);
}

void u2_printf(char *fmt,...)
{
	char buffer[50];
	uint16_t i=0;
	va_list arg_ptr;
	va_start(arg_ptr,fmt);
	vsnprintf(buffer,50,fmt,arg_ptr);
	while(i<50&&buffer[i])
	{
		while((USART2->SR&0x40)==0);
		USART_SendData(USART2,buffer[i]);
		i++;
	}
	va_end(arg_ptr);
}


void USART1_SendByte(uint8_t sByte)
{
	//�ȴ����ͻ�����Ϊ�գ�ȷ����һ�������Ѿ��ɹ����ͳ�ȥ�������������ݶ�ʧ�������ݸ��ǣ�0��Ϊ�գ�1Ϊ��
	while(USART_GetFlagStatus (USART1, USART_FLAG_TXE) == 0);
	//��������
	USART_SendData (USART1, sByte);
}
/*
�ض��򴮿�
*/
int fputc(int ch,FILE *f)
{
	USART1_SendByte(ch);
	return ch;
}



