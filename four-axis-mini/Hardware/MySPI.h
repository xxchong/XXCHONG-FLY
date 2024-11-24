#ifndef __MYSPI_H
#define __MYSPI_H
#include "stm32f10x.h"                  // Device header






#define NFR_IRQ 	GPIO_Pin_2 /*GPIOB*/
#define MISO 		GPIO_Pin_14 /*GPIOB*/
#define MOSI 		GPIO_Pin_15 /*GPIOB*/
#define NFR_CE  	GPIO_Pin_8/*GPIOA*/
#define SCK  		GPIO_Pin_13 /*GPIOB*/
#define NFR_CSN 	GPIO_Pin_12/*GPIOB*/


uint8_t MySPI_R_IRQ(void) ;
void MySPI_W_IRQ(uint8_t BitValue) ;
void MySPI_W_CE(uint8_t BitValue);
void MySPI_W_CSN(uint8_t BitValue);
void MySPI_W_MOSI(uint8_t BitValue);
void MySPI_W_SCK(uint8_t BitValue);
uint8_t MySPI_R_MISO(void) ;
void MySPI_Init(void);
void MySPI_Start(void);
void MySPI_Stop(void);
uint8_t MySPI_SwapByte(uint8_t SendByte);




u8 SPI_Read_Buf(u8 reg, u8 *pBuf, u8 bytes);
u8 SPI_Read_Byte(u8 reg);
u8 SPI_Write_Buf(u8 reg,u8 *pBuf, u8 bytes);
u8 SPI_Write_Byte(u8 reg, u8 value);

#endif
