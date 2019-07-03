#include "bsp_wm8978.h"
#include <includes.h>


#define PLAYER_AUDIO_BUFF_SIZE  		5

#define PLAYER_POOL_SIZE						32
#define PLAYING_AUDIO_BGM_MAX				8
#define PLAYING_AUDIO_EFFECT_MAX		16

uint16_t 	playerAudioBuffA[PLAYER_AUDIO_BUFF_SIZE], 
					playerAudioBuffB[PLAYER_AUDIO_BUFF_SIZE];
					
uint16_t 	readAudioBuffer[PLAYER_AUDIO_BUFF_SIZE];
					
uint8_t 	playerAudioBufflag, playerAudioBuffIsread;

player_queue_item_t _playerPool[ PLAYER_POOL_SIZE ];

player_queue_t playerQueue;
player_queue_t playingEffectQueue;
player_queue_t playingBGMQueue;
uint32_t s_player_id = 1;

void player_queue_push( player_queue_t * p_player_queue, player_queue_item_t * p_item )
{
		player_queue_item_t * t_last = p_player_queue->last;
		if( t_last == NULL )
		{
				p_player_queue->frist = p_item;
				
		}else{
				t_last->next = p_item;
		}
		
		p_player_queue->last = p_item;
}

player_queue_item_t * player_queue_pop( player_queue_t * p_player_queue )
{
		player_queue_item_t * t_frist = p_player_queue->frist;
		
		if( t_frist == NULL )
		{
				return NULL;
		}
		
		p_player_queue->frist = t_frist->next;
		if( p_player_queue->last == t_frist )
		{
				p_player_queue->last = NULL;
		}
		
		return t_frist;
}

uint16_t player_queue_length( player_queue_t * p_player_queue )
{
		uint16_t t_length = 0;
	
		player_queue_item_t * t_item = p_player_queue->frist;
		for( ;; )
		{
				if( t_item == NULL ) break;
				t_item = t_item->next;
				t_length++;
		}
		
		return t_length;
}

uint32_t play_wav( const wav_t * p_wav, const uint8_t p_playType, const int16_t p_loops )
{
		player_queue_item_t * t_playerItem = NULL;
		player_queue_item_t * t_tmpPlayer = NULL;
		uint16_t t_length;
	
		player_queue_t * t_playerQueue = ( p_playType == PLAY_TYPE_BGM ? &playingBGMQueue : &playingEffectQueue );

		t_length = player_queue_length( &playingBGMQueue );
		if( t_length >= ( p_playType == PLAY_TYPE_BGM ? PLAYING_AUDIO_BGM_MAX : PLAYING_AUDIO_EFFECT_MAX ) )
		{
				t_tmpPlayer = player_queue_pop( t_playerQueue );
				
				if( t_tmpPlayer != NULL )
				{
						if( t_tmpPlayer->player.playStateChangeFunc != NULL )
						{
								//----------- play state
								t_tmpPlayer->player.playStateChangeFunc( t_tmpPlayer->player.playId, 0 );
						}
						
						player_queue_push( &playerQueue, t_tmpPlayer );
				}
		}
		
		t_playerItem = player_queue_pop( &playerQueue );
		
		if( t_playerItem == NULL )
		{
				return 0;
		}
		
		t_playerItem->player.wav = p_wav;
		t_playerItem->player.playOffset = 0;
		t_playerItem->player.loop = p_loops;
		t_playerItem->player.playType = p_playType;
		t_playerItem->player.playStateChangeFunc = NULL;
		t_playerItem->player.playId = s_player_id++;
		t_playerItem->next = NULL;
		
		player_queue_push( t_playerQueue, t_playerItem );
		
		return t_playerItem->player.playId;
}

uint8_t readData( player_t * p_player, uint16_t * p_buffer )
{
		int32_t t_bufferSize = PLAYER_AUDIO_BUFF_SIZE * 2;
		int32_t t_readSize = ( p_player->wav->data.size - p_player->playOffset ) >= t_bufferSize ? t_bufferSize : p_player->wav->data.size - p_player->playOffset;
	
		if( t_readSize <= 0 )
		{
				return 0;
		}
		
		if( t_readSize != t_bufferSize )
		{
				memset( p_buffer, 0,  t_bufferSize );
		}
		memcpy( p_buffer, p_player->wav->data_buffer + p_player->playOffset, t_readSize );
		
		p_player->playOffset += t_readSize;
		
		return 1;
}
// p_volume 0~255
void useVolume( int16_t * p_inout, const uint8_t p_volume )
{
		uint16_t i;
		for( i = 0; i < PLAYER_AUDIO_BUFF_SIZE; ++i )
		{
				p_inout[i] = p_inout[i] * p_volume / 0xFF;
		}
}

void mixer( int16_t * p_inout, const int16_t * p_in )
{
		uint16_t i;
		for( i = 0; i < PLAYER_AUDIO_BUFF_SIZE; ++i )
		{				
				p_inout[i] = p_inout[i] * ( INT16_MAX - abs( p_in[i] ) ) / INT16_MAX + 
										 p_in[i] * ( INT16_MAX - abs( p_inout[i] ) ) / INT16_MAX;
				
		}
}
void MusicPlayer_I2S_DMA_TX_Callback(void)
{
		if(I2Sx_TX_DMA_STREAM->CR&(1<<19))
		{
				playerAudioBufflag = 0;
		}
		else
		{
				playerAudioBufflag = 1;
		}
		playerAudioBuffIsread = 1;
}


void I2S_Int_Init( void )
{
		memset( playerAudioBuffA, 0, PLAYER_AUDIO_BUFF_SIZE * 2 );
		memset( playerAudioBuffB, 0, PLAYER_AUDIO_BUFF_SIZE * 2 );
	
		wm8978_Reset();
				

		wm8978_CfgAudioPath( DAC_ON, EAR_LEFT_ON | EAR_RIGHT_ON );
		wm8978_SetOUT1Volume( 63 );		
		
		wm8978_CfgAudioIF( I2S_Standard_Phillips, 16 );
	
		I2S_Stop();
		I2S_GPIO_Config();
				
		I2S_DMA_TX_Callback = MusicPlayer_I2S_DMA_TX_Callback;
		
		I2Sx_Mode_Config( I2S_Standard_Phillips, I2S_DataFormat_16b, I2S_AudioFreq_44k );
		
		I2Sx_TX_DMA_Init( playerAudioBuffA, playerAudioBuffB, PLAYER_AUDIO_BUFF_SIZE );		
		I2S_Play_Start();
}

void TIM2_Int_Init( void )
{
		TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
     
		TIM_DeInit(TIM2);
     
		TIM_TimeBaseStructure.TIM_Period = 10 - 1;
		TIM_TimeBaseStructure.TIM_Prescaler = (36 / 4 - 1);
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
     
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
     
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);
		TIM_ITConfig(TIM2, TIM_IT_Update | TIM_IT_Trigger, ENABLE);
     
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		
		NVIC_Init( &NVIC_InitStructure );
		TIM_Cmd(TIM2, ENABLE);
}


void player_Init( void )
{
		uint16_t i;
		I2S_Int_Init();
		TIM2_Int_Init();
	
		playingEffectQueue.frist = NULL;
		playingEffectQueue.last = NULL;
	
		playingBGMQueue.frist = NULL;
		playingBGMQueue.last = NULL;
	
		for( i = 0; i < PLAYER_POOL_SIZE; ++i )
		{
				player_queue_push( &playerQueue, &_playerPool[i] );
		}
}

void TIM2_IRQHandler( void )
{
		uint8_t i;
		uint16_t * t_buff;
		uint8_t t_index = 0;
		player_queue_t * t_playerQueue;
		player_queue_item_t * t_player, * t_prvePlayer;
		
		if( TIM_GetITStatus( TIM2, TIM_IT_Update ) != RESET )
		{
				TIM_ClearITPendingBit( TIM2, TIM_IT_Update );
				
				if( playerAudioBuffIsread )
				{
						playerAudioBuffIsread = 0;
						t_buff = playerAudioBufflag ? playerAudioBuffB : playerAudioBuffA;
					
						memset( t_buff, 0, PLAYER_AUDIO_BUFF_SIZE * 2 );
					
						for( i = 0; i < 2; ++i )
						{
								if( i == 0 ) t_playerQueue = &playingBGMQueue;
								if( i == 1 ) t_playerQueue = &playingEffectQueue;
							
								t_prvePlayer = NULL;
								t_player = t_playerQueue->frist;
								while( t_player != NULL )
								{
										if( !readData( &( t_player->player ), readAudioBuffer ) )
										{
												if( t_prvePlayer == NULL )
												{
														t_playerQueue->frist = t_player->next;
														if( t_playerQueue->frist == NULL )
														{
																t_playerQueue->last = NULL;
														}
												}else{
														t_prvePlayer->next = t_player->next;
														
														if( t_prvePlayer->next == NULL )
														{
																t_playerQueue->last = t_prvePlayer;
														}
												}
												
												player_queue_push( &playerQueue, t_player );
												t_player = t_player->next;
												continue;
										}
										if( !t_index )
										{
												memcpy( t_buff, readAudioBuffer, PLAYER_AUDIO_BUFF_SIZE * 2 );
										}else{
												mixer( (int16_t*)t_buff, (int16_t*)readAudioBuffer );
										}
										t_index++;
										t_prvePlayer = t_player;
										t_player = t_player->next;
										
								}
								
								useVolume( (int16_t *)t_buff, 125 );
						}
				}
		}
		
}
