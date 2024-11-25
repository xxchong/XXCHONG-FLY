#ifndef __LED_H
#define __LED_H

#define LED1 GPIO_Pin_3
#define LED4 GPIO_Pin_4
#define LED5 GPIO_Pin_5
#define LED2 GPIO_Pin_6
#define LED3 GPIO_Pin_7

void LED_Init(void);
void LED_On(u16 LEDx);
void LED_Off(u16 LEDx);
void LED_Toggle(u16 LEDx);

#endif
