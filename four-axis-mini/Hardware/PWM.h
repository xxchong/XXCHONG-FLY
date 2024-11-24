#ifndef __PWM_H
#define __PWM_H

#include "stm32f10x.h"                  // Device header

void TIM3_PWM_Init(void);
void Motor_Set_Pwm(uint16_t PWM1,uint16_t PWM2,uint16_t PWM3,uint16_t PWM4);
void Motor_Pwm_Limit(uint16_t *PWM1,uint16_t *PWM2,uint16_t *PWM3,uint16_t *PWM4);

#endif
