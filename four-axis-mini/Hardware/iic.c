#include "sys.h"
#include "iic.h"

#undef		SUCCESS
#define		SUCCESS		0
#undef		FAILED
#define		FAILED		1

/******************************************************************************
 * 函数名称: I2c_delay
 * 函数功能: I2c 延时函数
 * 入口参数: 无
 
 
 在宏定义中使用反斜杠 \ 是为了将宏的定义延续到下一行。在C和C++中，预处理器指令
 （比如 #define）通常需要在同一行内完成。但是，当你需要定义一个跨越多行的宏时，
 你可以使用反斜杠 \ 来告诉预处理器，宏的定义将在下一行继续。

 ******************************************************************************/
#define I2c_delay()		{\
							volatile unsigned char i = 1;\
							while(i) i--;\
						}

/******************************************************************************
 * 函数名称: I2c_Init
 * 函数功能: I2c  GPIO初始化
 * 入口参数: 无
 ******************************************************************************/
 void IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStrucSUCCESS;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	    
    GPIO_InitStrucSUCCESS.GPIO_Pin = SCL_PIN | SDA_PIN;
    GPIO_InitStrucSUCCESS.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStrucSUCCESS.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(IIC_GPIO, &GPIO_InitStrucSUCCESS);
}

/******************************************************************************
 * 函数名称: I2c_Start
 * 函数功能: I2c  起始信号
 * 入口参数: 无
 ******************************************************************************/
static uint8_t I2c_Start(void)
{
    SDA_H;
    SCL_H;
	I2c_delay();
    if(!SDA_read)//如果读到的电平为低，则失败
	{
        return FAILED;
	}
    SDA_L;
    I2c_delay();
    if(SDA_read)
	{
        return FAILED;
	}
    SCL_L;
    I2c_delay();
    return SUCCESS;
}

/******************************************************************************
 * 函数名称: I2c_Stop
 * 函数功能: I2c  停止信号
 * 入口参数: 无
 ******************************************************************************/
static void I2c_Stop(void)
{
    SCL_L;
    I2c_delay();
    SDA_L;
	I2c_delay();
    I2c_delay();
    SCL_H;
	I2c_delay();
    SDA_H;
    I2c_delay();
}

/******************************************************************************
 * 函数名称: I2c_Ack
 * 函数功能: I2c  产生应答信号
 * 入口参数: 无
 ******************************************************************************/
static void I2c_Ack(void)
{
    SCL_L;
    I2c_delay();
    SDA_L;
    I2c_delay();
    SCL_H;
	I2c_delay();
	I2c_delay();
	I2c_delay();
    I2c_delay();
    SCL_L;
    I2c_delay();
}

/******************************************************************************
 * 函数名称: I2c_NoAck
 * 函数功能: I2c  产生NAck
 * 入口参数: 无
 ******************************************************************************/
static void I2c_NoAck(void)
{
    SCL_L;
    I2c_delay();
    SDA_H;
    I2c_delay();
    SCL_H;
	I2c_delay();
	I2c_delay();
	I2c_delay();
    I2c_delay();
    SCL_L;
    I2c_delay();
}

/*******************************************************************************
 *函数名称:	I2c_WaitAck
 *函数功能:	等待应答信号到来
 *返回值：   1，接收应答失败
 *           0，接收应答成功
 *******************************************************************************/
static uint8_t I2c_WaitAck(void)
{
    SCL_L;
    I2c_delay();
    SDA_H;
    I2c_delay();
    SCL_H;
	I2c_delay();
	I2c_delay();
    I2c_delay();
	
    if(SDA_read) 
	{
        SCL_L;
        return FAILED;
    }
    SCL_L;
    return SUCCESS;
}

/******************************************************************************
 * 函数名称: I2c_SendByte
 * 函数功能: I2c  发送一个字节数据
 * 入口参数: byte  发送的数据
 ******************************************************************************/
static void I2c_SendByte(uint8_t byte)
{
    uint8_t i = 8;
    while(i--) 
	{
        SCL_L;
        I2c_delay();
        if(byte & 0x80) SDA_H;
        else SDA_L;
        byte <<= 1;
        I2c_delay();
        SCL_H;
		I2c_delay();
		I2c_delay();
		I2c_delay();
    }
    SCL_L;
}

/******************************************************************************
 * 函数名称: I2c_ReadByte
 * 函数功能: I2c  读取一个字节数据
 * 入口参数: 无
 * 返回值	 读取的数据
 ******************************************************************************/
static uint8_t I2c_ReadByte(void)
{
    uint8_t i = 8;
    uint8_t byte = 0;

    SDA_H;
    while(i--) 
	{
        byte <<= 1;
        SCL_L;
        I2c_delay();
		I2c_delay();
        SCL_H;
		I2c_delay();
        I2c_delay();
		I2c_delay();
        if(SDA_read) 
		{
            byte |= 0x01;
        }
    }
    SCL_L;
    return byte;
}

/******************************************************************************
 * 函数名称: i2cWriteBuffer
 * 函数功能: I2c       向设备的某一个地址写入固定长度的数据
 * 入口参数: addr,     设备地址
 *           reg，     寄存器地址
 *			 len，     数据长度
 *			 *data	   数据指针
 * 返回值	 1
 ******************************************************************************/
int8_t IIC_Write_Bytes(uint8_t addr,uint8_t reg,uint8_t *data,uint8_t len)
{
    int i;
    if(I2c_Start() == FAILED)  return FAILED;
    I2c_SendByte(addr);
    if(I2c_WaitAck() == FAILED) 
	{
        I2c_Stop();
        return FAILED;
    }
    I2c_SendByte(reg);
    I2c_WaitAck();
    for(i = 0; i < len; i++) 
	{
        I2c_SendByte(data[i]);
        if(I2c_WaitAck() == FAILED) 
		{
            I2c_Stop();
            return FAILED;
        }
    }
    I2c_Stop();
    return SUCCESS;
}
int8_t IIC_Read_One_Byte(uint8_t addr,uint8_t reg)
{
	uint8_t recive = 0;
    if(I2c_Start() == FAILED) return FAILED;
    I2c_SendByte(addr);
    if(I2c_WaitAck() == FAILED) 
	{
        I2c_Stop();
        return FAILED;
    }
    I2c_SendByte(reg);
    I2c_WaitAck();
	I2c_Stop();
    I2c_Start();
	I2c_SendByte(addr+1);
    if(I2c_WaitAck() == FAILED) 
	{
        I2c_Stop();
        return FAILED;
    }
	recive = I2c_ReadByte();
	I2c_NoAck();
	I2c_Stop();
	return recive;
}

/*****************************************************************************
 *函数名称:	i2cWrite
 *函数功能:	写入指定设备 指定寄存器一个字节
 *入口参数： addr 目标设备地址
 *		     reg   寄存器地址
 *		     data 读出的数据将要存放的地址
 *******************************************************************************/
int8_t IIC_Write_One_Byte(uint8_t addr,uint8_t reg,uint8_t data)
{
    if(I2c_Start() == FAILED) return FAILED;
    I2c_SendByte(addr);
    if(I2c_WaitAck() == FAILED) 
	{
        I2c_Stop();
        return FAILED;
    }
    I2c_SendByte(reg);
    I2c_WaitAck();
    I2c_SendByte(data);
    I2c_WaitAck();
    I2c_Stop();
    return SUCCESS;
}

int8_t IIC_read_Bytes(uint8_t addr,uint8_t reg,uint8_t *data,uint8_t len)
{
    if (I2c_Start() == FAILED) return FAILED;
    I2c_SendByte(addr);
    if(I2c_WaitAck() == FAILED) 
	{
        I2c_Stop();
        return FAILED;
    }
    I2c_SendByte(reg);
    I2c_WaitAck();
	I2c_Stop();
    I2c_Start();
    I2c_SendByte(addr+1);
    if(I2c_WaitAck() == FAILED) 
	{
        I2c_Stop();
        return FAILED;
    }
    while(len) 
	{
        *data = I2c_ReadByte();
        if(len == 1) I2c_NoAck();
        else I2c_Ack();
        data++;
        len--;
    }
    I2c_Stop();
    return SUCCESS;
}