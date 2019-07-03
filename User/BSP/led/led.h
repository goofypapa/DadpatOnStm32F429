#ifndef __LED_H__
#define __LED_H__

#include <stdint.h>

#define LED1_INT_GPIO_PORT       GPIOB
#define LED1_INT_GPIO_CLK        RCC_AHB1Periph_GPIOB
#define LED1_INT_GPIO_PIN        GPIO_Pin_0

#define LED2_INT_GPIO_PORT       GPIOB
#define LED2_INT_GPIO_CLK        RCC_AHB1Periph_GPIOB
#define LED2_INT_GPIO_PIN        GPIO_Pin_1

#define LED3_INT_GPIO_PORT       GPIOB
#define LED3_INT_GPIO_CLK        RCC_AHB1Periph_GPIOB
#define LED3_INT_GPIO_PIN        GPIO_Pin_5

#define LED4_INT_GPIO_PORT       GPIOB
#define LED4_INT_GPIO_CLK        RCC_AHB1Periph_GPIOB
#define LED4_INT_GPIO_PIN        GPIO_Pin_8

#define LED5_INT_GPIO_PORT       GPIOB
#define LED5_INT_GPIO_CLK        RCC_AHB1Periph_GPIOB
#define LED5_INT_GPIO_PIN        GPIO_Pin_9

#define LED6_INT_GPIO_PORT       GPIOB
#define LED6_INT_GPIO_CLK        RCC_AHB1Periph_GPIOB
#define LED6_INT_GPIO_PIN        GPIO_Pin_10

#define LED7_INT_GPIO_PORT       GPIOB
#define LED7_INT_GPIO_CLK        RCC_AHB1Periph_GPIOB
#define LED7_INT_GPIO_PIN        GPIO_Pin_11

#define LED8_INT_GPIO_PORT       GPIOB
#define LED8_INT_GPIO_CLK        RCC_AHB1Periph_GPIOB
#define LED8_INT_GPIO_PIN        GPIO_Pin_13

#define LED9_INT_GPIO_PORT       GPIOB
#define LED9_INT_GPIO_CLK        RCC_AHB1Periph_GPIOB
#define LED9_INT_GPIO_PIN        GPIO_Pin_14

#define LED10_INT_GPIO_PORT       GPIOB
#define LED10_INT_GPIO_CLK        RCC_AHB1Periph_GPIOB
#define LED10_INT_GPIO_PIN        GPIO_Pin_15

#define LED11_INT_GPIO_PORT       GPIOD
#define LED11_INT_GPIO_CLK        RCC_AHB1Periph_GPIOD
#define LED11_INT_GPIO_PIN        GPIO_Pin_5

#define LED12_INT_GPIO_PORT       GPIOD
#define LED12_INT_GPIO_CLK        RCC_AHB1Periph_GPIOD
#define LED12_INT_GPIO_PIN        GPIO_Pin_6

#define LED13_INT_GPIO_PORT       GPIOD
#define LED13_INT_GPIO_CLK        RCC_AHB1Periph_GPIOD
#define LED13_INT_GPIO_PIN        GPIO_Pin_7

#define LED14_INT_GPIO_PORT       GPIOD
#define LED14_INT_GPIO_CLK        RCC_AHB1Periph_GPIOD
#define LED14_INT_GPIO_PIN        GPIO_Pin_11

#define LED15_INT_GPIO_PORT       GPIOD
#define LED15_INT_GPIO_CLK        RCC_AHB1Periph_GPIOD
#define LED15_INT_GPIO_PIN        GPIO_Pin_12

#define LED16_INT_GPIO_PORT       GPIOD
#define LED16_INT_GPIO_CLK        RCC_AHB1Periph_GPIOD
#define LED16_INT_GPIO_PIN        GPIO_Pin_13

#define LED17_INT_GPIO_PORT       GPIOE
#define LED17_INT_GPIO_CLK        RCC_AHB1Periph_GPIOE
#define LED17_INT_GPIO_PIN        GPIO_Pin_2

#define LED18_INT_GPIO_PORT       GPIOE
#define LED18_INT_GPIO_CLK        RCC_AHB1Periph_GPIOE
#define LED18_INT_GPIO_PIN        GPIO_Pin_3

#define LED19_INT_GPIO_PORT       GPIOE
#define LED19_INT_GPIO_CLK        RCC_AHB1Periph_GPIOE
#define LED19_INT_GPIO_PIN        GPIO_Pin_4

#define LED20_INT_GPIO_PORT       GPIOE
#define LED20_INT_GPIO_CLK        RCC_AHB1Periph_GPIOE
#define LED20_INT_GPIO_PIN        GPIO_Pin_5

#define LED21_INT_GPIO_PORT       GPIOE
#define LED21_INT_GPIO_CLK        RCC_AHB1Periph_GPIOE
#define LED21_INT_GPIO_PIN        GPIO_Pin_6


void LED_Init( void );

void LED_ON( uint8_t p_id );
void LED_OFF( uint8_t p_id );

void LED_ON_All( void );
void LED_OFF_All( void );


#endif //__LED_H__
