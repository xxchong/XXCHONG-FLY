  
#include "sys.h" 


uint8_t Serial_RxData;
uint8_t receivedata;
uint8_t Serial_RxFlag;

void Serial_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIOA_InitStruct;
	GPIOA_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIOA_InitStruct.GPIO_Pin=GPIO_Pin_9;
	GPIOA_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIOA_InitStruct);
	
	GPIOA_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIOA_InitStruct.GPIO_Pin=GPIO_Pin_10;
	GPIOA_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIOA_InitStruct);
	
	USART_InitTypeDef USART1_InitTypeDef;
	USART1_InitTypeDef.USART_BaudRate=460800;
	USART1_InitTypeDef.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//硬件流控制
	USART1_InitTypeDef.USART_Mode=USART_Mode_Tx | USART_Mode_Rx;
	USART1_InitTypeDef.USART_Parity=USART_Parity_No;//校验位
	USART1_InitTypeDef.USART_StopBits=USART_StopBits_1;
	USART1_InitTypeDef.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART1,&USART1_InitTypeDef);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);		//串口空闲中断
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);		//使能串口DMA发送
	USART_Cmd(USART1,ENABLE);

}

void USART1_DMA_Init(DMA_Channel_TypeDef* DMA1_CHx,uint32_t Peripheral_addr,uint32_t *Memory_addr,uint16_t Data_length)
{
	
	DMA_InitTypeDef DMA_InitStruct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	
	DMA_DeInit(DMA1_CHx);		//复位
	DMA_InitStruct.DMA_BufferSize=Data_length;        //发送数据长度
	DMA_InitStruct.DMA_DIR=DMA_DIR_PeripheralDST;     //数据传输方向，从内存发送到外设
	DMA_InitStruct.DMA_M2M=DMA_M2M_Disable;
	DMA_InitStruct.DMA_MemoryBaseAddr=(uint32_t)Memory_addr;
	DMA_InitStruct.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
	DMA_InitStruct.DMA_MemoryInc=DMA_MemoryInc_Enable;//设置内存地址是否应该在每次数据传输后自动递增
	DMA_InitStruct.DMA_Mode=DMA_Mode_Normal;
	DMA_InitStruct.DMA_PeripheralBaseAddr=Peripheral_addr;
	DMA_InitStruct.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_Priority=DMA_Priority_VeryHigh;
	DMA_Init(DMA1_CHx,&DMA_InitStruct);//TX

}

void USART1_DMA_Send(uint8_t *DATA,uint16_t Lenght)
{
	DMA_Cmd(DMA1_Channel4,DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel4,Lenght); 
	DMA_Cmd(DMA1_Channel4,ENABLE);

}


void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1,Byte);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
}

void Serial_SendArray(uint8_t *Array,uint16_t length)
{
	uint16_t i;
	for(i=0;i<length;i++)
	{
		Serial_SendByte(Array[i]);
	}
}
void Serial_SendString(char *String)
{
	while(*String != '\0')
	{
		Serial_SendByte(*String);
		String++;
	}
	
}

void Usart_SendString(u8 *String,uint8_t len)
{
	for(int i=0; i<len;i++)
	{
		Serial_SendByte(String[i]);
	}
	
}


uint32_t Serial_Pow(uint32_t X,uint32_t Y)
{
	uint32_t Result=1;
	while(Y--)
	{
		Result *= X;
	}		
	return Result;
}
void Serial_SendNumber(uint32_t Number,uint8_t Length)
{
	uint8_t i;
	for(i=0;i<Length;i++)
	{
		Serial_SendByte(Number / Serial_Pow(10,Length-i-1) %10 + '0');
	}
}

uint8_t Serial_GetRxFlag(void)
{
	if(Serial_RxFlag)
	{
		Serial_RxFlag=0;
		return 1;
	}
	return 0;
}
uint8_t Serial_GetRxData(void)
{
	return receivedata;
}

void USART1_IRQHandler(void)
{
	uint8_t clear = clear; //定义这个变量是针对编译出现“没有用到这个变量”的警告提示
	uint8_t res;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //接收中断
	{ 
		res = USART1->DR;
		ANO_DT_Data_Receive_Prepare(res); //上位机数据接收与解析
	}else if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) //空闲中断
	{
		clear = USART1->SR; //读SR寄存器
		clear = USART1->DR; //读DR寄存器（先读SR,再度DR,就是为了清除IDIE中断）
	}
	USART_ClearITPendingBit(USART1,USART_IT_RXNE);
}
//重定向
int fputc(int ch,FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}

void Serial_Printf(char *format,...)
{
	char String[100];
	va_list arg;
	va_start(arg,format);
	sprintf(String,format,arg);
	va_end(arg);
	Serial_SendString(String);
}
