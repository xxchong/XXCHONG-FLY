#ifndef __LED_H
#define __LED_H


#define LED1 GPIO_Pin_1
#define LED2 GPIO_Pin_2

#define LED1_ON GPIOA->BSRR = LED1
#define LED1_OFF GPIOA->BRR = LED1
#define LED1_Turn GPIOA->ODR & LED1 ? (GPIOA->BRR = LED1) : (GPIOA->BSRR = LED1)

#define LED2_ON GPIOA->BSRR = LED2
#define LED2_OFF GPIOA->BRR = LED2
#define LED2_Turn GPIOA->ODR & LED2 ? (GPIOA->BRR = LED2) : (GPIOA->BSRR = LED2)

void Led_Init(void);


#endif
