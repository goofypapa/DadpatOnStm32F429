#ifndef __PLAYER_H__
#define __PLAYER_H__


#include "wav.h"
#include <stdint.h>

#define PLAY_TYPE_BGM 0
#define PLAY_TYPE_EFFECT 1

typedef void( * PlayStateChangeFunc )( const uint32_t p_playId, const uint8_t p_playState );

typedef struct _player_t{
		const wav_t * wav;
		uint32_t playOffset;
		uint32_t playId;
		PlayStateChangeFunc playStateChangeFunc;
		int16_t loop;
		uint8_t playType;
} player_t;

typedef struct _player_queue_item_t{
		void * next;
		player_t player;
} player_queue_item_t;

typedef struct _player_queue_t{
		void * frist;
		void * last;
} player_queue_t;


void player_Init( void );

uint32_t play_wav( const wav_t * p_wav, const uint8_t p_playType, const int16_t p_loops );

uint8_t stop( const uint32_t p_playId );

uint8_t pause( const uint32_t p_playId );

uint8_t  resume( const uint32_t p_playId );

#endif //__PLAYER_H__
