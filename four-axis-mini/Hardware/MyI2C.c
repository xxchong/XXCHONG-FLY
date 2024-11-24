#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void MyI2C_W_SCL(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_10,(BitAction)BitValue);
	delay_us(10);
}

void MyI2C_W_SDA(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_11,(BitAction)BitValue);
	delay_us(10);
}

uint8_t MyI2C_R_SDA(void)
{
	uint8_t bitValue;
	bitValue=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11);
	delay_us(10);
	return bitValue;
}

void MyI2C_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitTypeDef GPIOB_InitStruct;
	GPIOB_InitStruct.GPIO_Mode=GPIO_Mode_Out_OD;//开漏输出
	GPIOB_InitStruct.GPIO_Pin=GPIO_Pin_10 | GPIO_Pin_11;//10为scl
	GPIOB_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIOB_InitStruct);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_10| GPIO_Pin_11);
}

void MyI2C_Start(void)
{
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(0);
	
}
void MyI2C_Stop(void)
{
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(1);
}
void MyI2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	
	for(i=0;i<8;i++)
	{
		MyI2C_W_SDA(Byte&(0x80 >> i));
		MyI2C_W_SCL(1);
		MyI2C_W_SCL(0);
	}
}
uint8_t MyI2C_ReceiveByte(void)
{
	uint8_t i,Byte=0x00;
	MyI2C_W_SDA(1);//接收前确保释放SDA
	for(i=0;i<8;i++)
	{
		MyI2C_W_SCL(1);
		if(	MyI2C_R_SDA() == 1)//为0则默认是0
		{
			Byte |= (0x80 >> i);
		}
		MyI2C_W_SCL(0);
	}
	return Byte;
}
void MyI2C_SendACK(uint8_t ACKBit)
{
	MyI2C_W_SDA(ACKBit);
	MyI2C_W_SCL(1);
	MyI2C_W_SCL(0);
	
}
uint8_t MyI2C_ReceiveACK(void)
{
	uint8_t ACKBit;
	MyI2C_W_SDA(1);//接收前确保释放SDA
	
	MyI2C_W_SCL(1);
	ACKBit = MyI2C_R_SDA();
	MyI2C_W_SCL(0);
	return ACKBit;
}
