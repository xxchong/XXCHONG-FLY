#include "SYS.h"     
#include "Delay.h"     

#define H_Bit GPIO_WriteBit(GPIOB,GPIO_Pin_0,(BitAction)(1))
#define L_Bit GPIO_WriteBit(GPIOB,GPIO_Pin_0,(BitAction)(0))


void HT11_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
}

void HT11_In_Mode()
{
	GPIO_InitTypeDef GPIOB_StructInit;
	GPIOB_StructInit.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIOB_StructInit.GPIO_Pin=GPIO_Pin_0;
	GPIOB_StructInit.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIOB_StructInit);
}
void HT11_Out_Mode()
{
	GPIO_InitTypeDef GPIOB_StructInit;
	
	GPIOB_StructInit.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOB_StructInit.GPIO_Pin=GPIO_Pin_0;
	GPIOB_StructInit.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIOB_StructInit);
}



void HT11_Start(void)
{
	HT11_Out_Mode();
	H_Bit;
	
	Delay_us(25);
	
	L_Bit;

	Delay_ms(20);

	
	H_Bit;

	Delay_us(25);

	
	
}

uint8_t HT11_Response(void)
{
	HT11_In_Mode();
	uint8_t timeout=50;
	//等待拉低
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0) == 1)
	{
		if(timeout == 0)
			return 0;
		
		timeout--;
		Delay_us(1);
	
	}
	timeout=50;
	//等待拉高
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0) == 0)
	{
		if(timeout == 0)
			return 0;
		
		timeout--;
		Delay_us(1);
	}
	return 1;
	
 
}


uint8_t HT11_ReadBit(void)
{
	
	uint8_t timeout=50;
	 // 等待从机拉低数据线
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0) == 1);
	timeout=50;
	 // 等待从机拉高数据线
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0) == 0);
	
	Delay_us(40);
	return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0);
	
	
}
//读取一个字节 8bit数据
uint8_t HT11_ReadByte(void)
{
	uint8_t Byte=0;
	for(int i=0;i<8;i++)
	{

		Byte |=HT11_ReadBit() << (7-i);
	}
	return Byte;

}


uint8_t DHT11_Read_Data(uint8_t *humiH,uint8_t *humiL,
uint8_t *tempH,uint8_t *tempL)
{
	uint8_t buf[5],i;
	HT11_Start();

	Delay_us(40);
	
	for(i=0; i<5;i++)
	{
	if(HT11_Response() == 1 )//检测到DHT11响应
  {
			for(i = 0; i < 5 ; i++) //读取40位字节
			{	
				buf[i] = HT11_ReadByte();
			}
			if((buf[0]+buf[1]+buf[2]+buf[3]) == buf[4])
			{
				*humiH=buf[0];
				*humiL=buf[1];
				*tempH=buf[2];
				*tempL=buf[3];
			}
	}
	}
}






