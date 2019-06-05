#ifndef __USART_H__
#define __USART_H__

#include "stdint.h"
#include "stm32f4xx_usart.h"

#define DEBUG_USART									USART2
#define DEBUG_USART_CLK							RCC_APB1Periph_USART2
#define DEBUG_USART_BAUDRATE				115200

#define DEBUG_USART_RX_GPIO_PORT		GPIOA
#define DEBUG_USART_RX_GPIO_CLK			RCC_AHB1Periph_GPIOA
#define DEBUG_USART_RX_PIN					GPIO_Pin_3
#define DEBUG_USART_RX_AF						GPIO_AF_USART2
#define DEBUG_USART_RX_SOURCE				GPIO_PinSource3

#define DEBUG_USART_TX_GPIO_PORT		GPIOA
#define DEBUG_USART_TX_GPIO_CLK			RCC_AHB1Periph_GPIOA
#define DEBUG_USART_TX_PIN					GPIO_Pin_2
#define DEBUG_USART_TX_AF						GPIO_AF_USART2
#define DEBUG_USART_TX_SOURCE				GPIO_PinSource2

#define DEBUG_USART_IRQHandler			USART2_IRQHandler
#define DEBUG_USART_IRQ							USART2_IRQn

//DMA
#define DEBUG_USART_DR_BASE					(USART2_BASE + 0x04)
#define DEBUG_USART_DMA_CLK					RCC_AHB1Periph_DMA2
#define DEBUG_USART_DMA_CHANNEL			DMA_Channel_4
#define DEBUG_USART_DMA_STREAM			DMA2_Stream7


#define DEBUG_USART_DMA_SENDBUFFER_SIZE			5000

void Debug_USART_Config( void );
void Debug_USART_DMA_Config( void );

void Usart_SendByte( USART_TypeDef * p_USARTx, uint8_t p_byte );
void Usart_SendString( USART_TypeDef * p_USARTx, char * p_string );

void Usart_RecvCmd( char * p_cmd );

#endif //__USART_H__
