#include <includes.h>

#define CMD_BUFFER_SZIE 128

char _CmdBuffer[2][CMD_BUFFER_SZIE];
char * CmdBuffer = _CmdBuffer[0];
int32_t CmdRecvIndex = 0;
int32_t CmdLen = 0;

uint8_t DmaSendBuffer[ DEBUG_USART_DMA_SENDBUFFER_SIZE ];

static void NVIC_Configuration( void )
{
		NVIC_InitTypeDef NVIC_InitStructure;
	
		NVIC_PriorityGroupConfig( NVIC_PriorityGroup_2 );
	
		NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
	
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
		NVIC_Init( &NVIC_InitStructure );
	
}

void Debug_USART_Config( void )
{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
	
		RCC_AHB1PeriphClockCmd( DEBUG_USART_RX_GPIO_CLK | DEBUG_USART_TX_GPIO_CLK, ENABLE );
		RCC_APB1PeriphClockCmd( DEBUG_USART_CLK, ENABLE );
	
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_PIN;
		GPIO_Init( DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure );
	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_PIN;
		GPIO_Init( DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure );
	
		GPIO_PinAFConfig( DEBUG_USART_RX_GPIO_PORT, DEBUG_USART_RX_SOURCE, DEBUG_USART_RX_AF );
		GPIO_PinAFConfig( DEBUG_USART_TX_GPIO_PORT, DEBUG_USART_TX_SOURCE, DEBUG_USART_TX_AF );
	
		USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init( DEBUG_USART, &USART_InitStructure );
		
		NVIC_Configuration();
		
		USART_ITConfig( DEBUG_USART, USART_IT_RXNE, ENABLE );
		USART_Cmd( DEBUG_USART, ENABLE );
				
}

void Debug_USART_DMA_Config( void )
{
		DMA_InitTypeDef DMA_InitStructure;
		
		RCC_AHB1PeriphClockCmd( DEBUG_USART_DMA_CLK, ENABLE );
		DMA_DeInit( DEBUG_USART_DMA_STREAM );
		while( DMA_GetCmdStatus( DEBUG_USART_DMA_STREAM ) != DISABLE );
	
		DMA_InitStructure.DMA_Channel = DEBUG_USART_DMA_CHANNEL;
		DMA_InitStructure.DMA_PeripheralBaseAddr = DEBUG_USART_DR_BASE;
		DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)DmaSendBuffer;
		DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
		DMA_InitStructure.DMA_BufferSize = DEBUG_USART_DMA_SENDBUFFER_SIZE;
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
		DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
		
		DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
		DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
		DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		DMA_Init( DEBUG_USART_DMA_STREAM, &DMA_InitStructure );

		DMA_Cmd( DEBUG_USART_DMA_STREAM, ENABLE );
		while( DMA_GetCmdStatus( DEBUG_USART_DMA_STREAM ) != ENABLE );
}

void Usart_SendByte( USART_TypeDef * p_USARTx, uint8_t p_byte )
{
		USART_SendData( p_USARTx, p_byte );
	
		while( USART_GetFlagStatus( p_USARTx, USART_FLAG_TXE ) == RESET );
}

void Usart_SendString( USART_TypeDef * p_USARTx, char * p_string )
{
		unsigned int i = 0;
		
		do{
				Usart_SendByte( p_USARTx, *( p_string + i ) );
		}while( *(p_string + i++) != '\0' );
		
		while( USART_GetFlagStatus( p_USARTx, USART_FLAG_TXE ) == RESET );
}


void DEBUG_USART_IRQHandler(void)
{
		uint8_t t_uc;
		
		if( USART_GetITStatus( DEBUG_USART, USART_IT_RXNE ) != RESET )
		{
				t_uc = USART_ReceiveData( DEBUG_USART );
			
				Usart_SendByte( DEBUG_USART, t_uc );
			
				if( t_uc == 0x08 )
				{
						CmdRecvIndex = CmdRecvIndex > 0 ? CmdRecvIndex - 1 : 0;
						return;
				}
			
				CmdBuffer[CmdRecvIndex] = t_uc;
				if( t_uc == '\r' || t_uc == '\n' )
				{
						if( t_uc == '\r' )
						{
								Usart_SendByte( DEBUG_USART, '\n' );
						}
						
						CmdBuffer[CmdRecvIndex + 1] = '\0';
						ShellCmdPut( CmdBuffer );
						CmdBuffer = CmdBuffer == _CmdBuffer[0] ? _CmdBuffer[1] : _CmdBuffer[0];
						CmdRecvIndex = 0;
						return;
				}
				
				CmdRecvIndex = CmdRecvIndex + 1 >= CMD_BUFFER_SZIE ? 0 : CmdRecvIndex + 1;
		}
}

int fputc(int ch, FILE *f)
{
		USART_SendData(DEBUG_USART, (uint8_t) ch);
		
		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

int fgetc(FILE *f)
{
		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USART);
}


