#include "SYS.h"               


//CE
void MySPI_W_CE(uint8_t BitValue) 
{
	GPIO_WriteBit(GPIOA,NFR_CE,(BitAction)BitValue);
}
//IRQ
void MySPI_W_IRQ(uint8_t BitValue) 
{
	GPIO_WriteBit(GPIOA,NFR_IRQ,(BitAction)BitValue);
}
//IRQ读取
uint8_t MySPI_R_IRQ(void) 
{
	uint8_t BitValue;
	BitValue=GPIO_ReadInputDataBit(GPIOA,NFR_IRQ);
	return BitValue;
}
//片选
void MySPI_W_CSN(uint8_t BitValue) 
{
	GPIO_WriteBit(GPIOB,NFR_CSN,(BitAction)BitValue);
}

//SCK
void MySPI_W_SCK(uint8_t BitValue) 
{
	GPIO_WriteBit(GPIOB,SCK,(BitAction)BitValue);
}

//MOSI
void MySPI_W_MOSI(uint8_t BitValue) 
{
   GPIO_WriteBit(GPIOA,MOSI,(BitAction)BitValue);
}
//MISO
uint8_t MySPI_R_MISO(void) 
{
    return GPIO_ReadInputDataBit(GPIOA,MISO);
}

//初始化	
void MySPI_Init(void)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
	GPIO_InitTypeDef GPIOA_InitStruct;
	GPIO_InitTypeDef GPIOB_InitStruct;
	EXTI_InitTypeDef EXTI_initStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	GPIOA_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出高低电平都具有驱动能力。
	GPIOA_InitStruct.GPIO_Pin=  NFR_CE | MOSI;
	GPIOA_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIOA_InitStruct);

	GPIOA_InitStruct.GPIO_Mode=GPIO_Mode_IPU;//上拉输入 MISO IRQ
	GPIOA_InitStruct.GPIO_Pin=NFR_IRQ|MISO;
	GPIOA_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIOA_InitStruct);
	
	GPIOB_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出高低电平都具有驱动能力。
	GPIOB_InitStruct.GPIO_Pin=SCK | NFR_CSN;
	GPIOB_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIOB_InitStruct);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource5);//开启GPIO管脚的中断线路
	EXTI_initStructure.EXTI_Line = EXTI_Line5;
	EXTI_initStructure.EXTI_LineCmd = ENABLE;
	EXTI_initStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_initStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//下降沿触发
	EXTI_Init(&EXTI_initStructure);
	NVIC_EnableIRQ(EXTI9_5_IRQn);
}
//开始
void MySPI_Start(void)
{
	MySPI_W_CSN(0);
}
//停止
void MySPI_Stop(void)
{
	MySPI_W_CSN(1);
}
//交换字节
uint8_t MySPI_SwapByte(uint8_t SendByte)
{	
		uint8_t receivedata=0x00,i;
		for(i=0;i<8;i++)
		{
		
		MySPI_W_MOSI(SendByte & ( 0x80 >> i));
		MySPI_W_SCK(1);//上升沿移入数据
		if(MySPI_R_MISO() == 1)
		{
			receivedata |= (0x80 >> i);
		}
		MySPI_W_SCK(0);//下降沿移出数据
	}
		return receivedata;
}



/********************************************************
函数功能：写寄存器的值（单字节）                
入口参数：reg:寄存器映射地址（格式：WRITE_REG｜reg）
					value:寄存器的值
返回  值：状态寄存器的值
*********************************************************/
u8 SPI_Write_Byte(u8 reg, u8 value)
{
	u8 status;

	MySPI_Start();                 
	status = MySPI_SwapByte(reg);				
	MySPI_SwapByte(value);
	MySPI_Stop();  
	
	return(status);
}

/********************************************************
函数功能：写寄存器的值（多字节）                  
入口参数：reg:寄存器映射地址（格式：WRITE_REG｜reg）
					pBuf:写数据首地址
					bytes:写数据字节数
返回  值：状态寄存器的值
*********************************************************/
u8 SPI_Write_Buf(u8 reg,u8 *pBuf, u8 bytes)
{
  u8 status,byte_ctr;
  MySPI_Start();                               			
  status = MySPI_SwapByte(reg);                          
  for(byte_ctr=0; byte_ctr<bytes; byte_ctr++)     
    MySPI_SwapByte(*pBuf++);
  MySPI_Stop();                                    	

  return(status);       
}							  					   

/********************************************************
函数功能：读取寄存器的值（单字节）                  
入口参数：reg:寄存器映射地址（格式：READ_REG｜reg）
返回  值：寄存器值
*********************************************************/
u8 SPI_Read_Byte(u8 reg)
{
 	u8 value;

	 MySPI_Start();       
	 MySPI_SwapByte(reg);			
	 value = MySPI_SwapByte(0);
	 MySPI_Stop();                                    	
	return(value);
}

/********************************************************
函数功能：读取寄存器的值（多字节）                  
入口参数：reg:寄存器映射地址（READ_REG｜reg）
					pBuf:接收缓冲区的首地址
					bytes:读取字节数
返回  值：状态寄存器的值
*********************************************************/
u8 SPI_Read_Buf(u8 reg, u8 *pBuf, u8 bytes)
{
 u8 status,byte_ctr;

  MySPI_Start();                                            
  status = MySPI_SwapByte(reg);                           
  for(byte_ctr=0;byte_ctr<bytes;byte_ctr++)
    pBuf[byte_ctr] = MySPI_SwapByte(0);                   //读取数据，低字节在前
  MySPI_Stop();                                        
  return(status);    
}


