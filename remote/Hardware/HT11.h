#ifndef __HT11_H
#define __HR11_H



void HT11_Init(void);

void HT11_In_Mode(void);

void HT11_Out_Mode(void);


void HT11_Start(void);


uint8_t HT11_Response(void);


uint8_t HT11_ReadBit(void);
uint8_t HT11_ReadByte(void);


uint8_t DHT11_Read_Data(uint8_t *humiH,uint8_t *humiL,
uint8_t *tempH,uint8_t *tempL);


#endif
