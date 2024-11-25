#ifndef _USART_H_
#define _USART_H_

#include "stm32f10x.h"



void USART2_DMA_Init(void);
void USART1_Config(uint32_t Baud);
void USART1_SendByte(uint8_t sByte);
void USART2_Config(uint32_t Baud);

#endif

