#ifndef __I2C_H
#define __I2C_H
#include "u8x8.h"
#include "u8g2.h"

#define SCL_Pin GPIO_Pin_8
#define SDA_Pin GPIO_Pin_9
#define IIC_GPIO_Port GPIOB

#define      OLED_SPIx                        SPI1
#define      OLED_SPI_APBxClock_FUN          RCC_APB2PeriphClockCmd
#define      OLED_SPI_CLK                     RCC_APB2Periph_SPI1
 
//SCK引脚
#define      OLED_SPI_SCK_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define      OLED_SPI_SCK_CLK                 RCC_APB2Periph_GPIOA   
#define      OLED_SPI_SCK_PORT                GPIOA   
#define      OLED_SPI_SCK_PIN                 GPIO_Pin_5
//MISO引脚
#define      OLED_SPI_MISO_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define      OLED_SPI_MISO_CLK                RCC_APB2Periph_GPIOA    
#define      OLED_SPI_MISO_PORT               GPIOA 
#define      OLED_SPI_MISO_PIN                GPIO_Pin_6
//MOSI引脚
#define      OLED_SPI_MOSI_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define      OLED_SPI_MOSI_CLK                RCC_APB2Periph_GPIOA    
#define      OLED_SPI_MOSI_PORT               GPIOA 
#define      OLED_SPI_MOSI_PIN                GPIO_Pin_7
 
//CS(NSS)引脚 片选选普通GPIO即可//可以拉低，或者悬空
#define      OLED_SPI_CS_APBxClock_FUN       RCC_APB2PeriphClockCmd
#define      OLED_SPI_CS_CLK                  RCC_APB2Periph_GPIOA  
#define      OLED_SPI_CS_PORT                 GPIOA
#define      OLED_SPI_CS_PIN                  GPIO_Pin_4
 
//RES引脚 片选选普通GPIO即可//可以接到系统复位引脚或者高电平
#define      OLED_SPI_RES_APBxClock_FUN       RCC_APB2PeriphClockCmd
#define      OLED_SPI_RES_CLK                  RCC_APB2Periph_GPIOB  
#define      OLED_SPI_RES_PORT                 GPIOB
#define      OLED_SPI_RES_PIN                  GPIO_Pin_0
 
//DC引脚 片选选普通GPIO即可
#define      OLED_SPI_DC_APBxClock_FUN       RCC_APB2PeriphClockCmd
#define      OLED_SPI_DC_CLK                  RCC_APB2Periph_GPIOB  
#define      OLED_SPI_DC_PORT                 GPIOB
#define      OLED_SPI_DC_PIN                  GPIO_Pin_1
 
#define  		OLED_CS_LOW()     						GPIO_ResetBits( OLED_SPI_CS_PORT, OLED_SPI_CS_PIN )
#define  		OLED_CS_HIGH()    						GPIO_SetBits( OLED_SPI_CS_PORT, OLED_SPI_CS_PIN )
 
#define         OLED_RST_Clr()                          GPIO_ResetBits(OLED_SPI_RES_PORT, OLED_SPI_DC_PIN)//RES
#define         OLED_RST_Set()                          GPIO_SetBits(OLED_SPI_RES_PORT, OLED_SPI_DC_PIN)
 
#define         OLED_DC_Clr()                           GPIO_ResetBits(OLED_SPI_DC_PORT, OLED_SPI_DC_PIN)//DC
#define         OLED_DC_Set()                           GPIO_SetBits(OLED_SPI_DC_PORT, OLED_SPI_DC_PIN)
 
 
 
void U8g2_Init(u8g2_t *u8g2);
void IIC_Init(void);
uint8_t u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
void u8g2Init(u8g2_t *u8g2);
void draw(u8g2_t *u8g2);


#endif
