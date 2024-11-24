#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>
void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array,uint16_t length);
void Serial_SendString(char *String);
void Serial_SendNumber(uint32_t Number,uint8_t Length);
void USART1_IRQHandler(void);
void Serial_Printf(char *format,...);

uint8_t Serial_GetRxFlag(void);
uint8_t Serial_GetRxData(void);
void Usart_SendString(u8 *String,uint8_t len);
void USART1_DMA_Send(uint8_t *DATA,uint16_t Lenght);

void USART1_DMA_Init(DMA_Channel_TypeDef* DMA1_CHx,uint32_t Peripheral_addr,uint32_t *Memory_addr,uint16_t Data_length);

#endif
