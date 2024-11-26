#include "stm32f10x.h"                  // Device header
#include "MySPI.h"                  // Device header

//#define NFR_IRQ 	GPIO_Pin_2 /*GPIOB*/
//#define MISO 		GPIO_Pin_14 /*GPIOB*/
//#define MOSI 		GPIO_Pin_15 /*GPIOB*/
//#define SCK  		GPIO_Pin_13 /*GPIOB*/
//#define NFR_CSN 	GPIO_Pin_12/*GPIOB*/


//#define NFR_CE  	GPIO_Pin_8/*GPIOA*/

void MySPI_W_CE(uint8_t BitValue) 
{
	GPIO_WriteBit(GPIOA,NFR_CE,(BitAction)BitValue);
}
void MySPI_W_IRQ(uint8_t BitValue) 
{
	GPIO_WriteBit(GPIOB,NFR_IRQ,(BitAction)BitValue);
}

uint8_t MySPI_R_IRQ(void) 
{
	uint8_t BitValue;
	BitValue=GPIO_ReadInputDataBit(GPIOB,NFR_IRQ);
	return BitValue;
}

void MySPI_W_CSN(uint8_t BitValue) 
{
	GPIO_WriteBit(GPIOB,NFR_CSN,(BitAction)BitValue);
}

void MySPI_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB| RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_initStructure;
	SPI_InitTypeDef SPI_InitStructure;


	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出高低电平都具有驱动能力。
	GPIO_InitStruct.GPIO_Pin= NFR_CSN;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;//硬件spi
	GPIO_InitStruct.GPIO_Pin=MOSI | SCK;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出高低电平都具有驱动能力。
	GPIO_InitStruct.GPIO_Pin=NFR_CE;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;//上拉输入 MISO IRQ
	GPIO_InitStruct.GPIO_Pin=NFR_IRQ | MISO;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource2);//开启GPIO管脚的中断线路
	EXTI_initStructure.EXTI_Line = EXTI_Line2;
	EXTI_initStructure.EXTI_LineCmd = ENABLE;
	EXTI_initStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_initStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//下降沿触发
	EXTI_Init(&EXTI_initStructure);
	NVIC_EnableIRQ(EXTI2_IRQn);

	 // SPI初始化
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; // 根据需要调整
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI2, &SPI_InitStructure);
    // 启用SPI
    SPI_Cmd(SPI2, ENABLE);
}
	
void MySPI_Start(void)
{
	MySPI_W_CSN(0);
}
void MySPI_Stop(void)
{
	MySPI_W_CSN(1);
}


uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) != SET);	//等待发送数据寄存器空
	
	SPI_I2S_SendData(SPI2, ByteSend);								//写入数据到发送数据寄存器，开始产生时序
	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) != SET);	//等待接收数据寄存器非空
	
	return SPI_I2S_ReceiveData(SPI2);								//读取接收到的数据并返回
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

