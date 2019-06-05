#include "wav.h"
#include "ff.h"
#include "sdram.h"
#include "stdio.h"
#include "string.h"


#define LOAD_WAV_START_ADDR (char *)SDRAM_BANK_ADDR
	
char * _load_wav_start_addr = LOAD_WAV_START_ADDR;

wav_t 		_load_wav_poll[1024];
uint16_t	_load_wav_curr_index = 0;

int16_t loadWav( wav_t ** p_wav, const char * p_filePath )
{
		char t_buffer[256];
		
		FRESULT t_result;
		FIL t_fil;
		uint32_t t_read_length, t_offset = 0;
		int32_t t_tmp_size;
		int16_t t_parse_result = -1;
		wav_t * t_wav = _load_wav_poll + _load_wav_curr_index;
		
		t_result = f_open( &t_fil, p_filePath, FA_READ );
		if( t_result != FR_OK )
		{
				printf( "load wav: %s: No such file \r\n", p_filePath );
				return t_parse_result;
		}
		
		do
		{
				t_result = f_read( &t_fil, t_buffer, 12, &t_read_length );
				if( t_result != FR_OK )
				{
						printf( "read wav file head final! \r\n" );
						break;
				}
				
				if( strncasecmp("RIFF", t_buffer, 4) )
        {
            printf( "file is not wav \r\n" );
            break;
        }
				
				memcpy(t_wav->riff.id, t_buffer, 4); 
        t_wav->riff.size = *(int *)(t_buffer + 4);
        if( strncasecmp("WAVE", t_buffer + 8, 4) )
        {
						printf( "Error wav file \r\n" );
            break;
        }
				
				memcpy(t_wav->riff.type, t_buffer + 8, 4);
        t_wav->file_size = t_wav->riff.size + 8;
        t_offset += 12;

				while( 1 )
				{
						t_result = f_read( &t_fil, t_buffer, 8, &t_read_length );
					
						if( t_result != FR_OK || t_read_length != 8 )
						{
								printf( "Error wav file \r\n" );
								break;
						}

						t_tmp_size = *(int32_t *)( t_buffer + 4 );
						if( !strncasecmp( "FMT", t_buffer, 3 ) )
						{
								memcpy( t_wav->format.id, t_buffer, 3 );
								t_wav->format.size = t_tmp_size;
								
								t_result = f_read( &t_fil, t_buffer, t_tmp_size, &t_read_length );
								
								if( t_result != FR_OK || t_read_length != t_tmp_size )
								{
										printf( "Error wav file \r\n" );
										break;
								}
								
								t_wav->format.compression_code = *(int16_t *)t_buffer;
								t_wav->format.channels = *(int16_t *)( t_buffer + 2 );
								t_wav->format.samples_per_sec = *(int32_t *)( t_buffer + 4 );
								t_wav->format.avg_bytes_per_sec = *(int32_t *)( t_buffer + 8 );
								t_wav->format.block_align = *(int16_t *)( t_buffer + 12 );
								t_wav->format.bits_per_sample = *(int16_t *)( t_buffer + 14 );
						}
						else if( !strncasecmp( "DATA", t_buffer, 3 ) )
						{
								memcpy( t_wav->format.id, t_buffer, 4 );
								t_wav->data.size = t_tmp_size - 8;
								t_offset += 8;
								t_wav->data_offset = t_offset;
								t_parse_result = _load_wav_curr_index;
								break;
						}else{
								t_result = f_lseek( &t_fil, t_offset + t_tmp_size );
							
								if( t_result != FR_OK )
								{
										printf( "------> seek final \r\n" );
								}
						}
						
						t_offset += t_tmp_size + 8;
				}
				
				if( t_parse_result < 0 )
				{
						break;
				}
				
				
				t_result = f_lseek( &t_fil, t_wav->data_offset );
				if( t_result != FR_OK )
				{
						printf( "------> seek final \r\n" );
				}
				
				t_result = f_read( &t_fil, _load_wav_start_addr, t_wav->data.size, &t_read_length );
				if( t_result != FR_OK || t_read_length != t_wav->data.size )
				{
						printf( "------> %d, %d", t_result == FR_OK, t_read_length );
				}
				
				t_wav->data_buffer = _load_wav_start_addr;
				_load_wav_start_addr += t_wav->data.size;
		}while(0);
		
		f_close( &t_fil );
		
		if( p_wav )
		{
			*p_wav = t_wav;
		}
		
		_load_wav_curr_index++;
		return t_parse_result;
}

uint8_t unloadWav( const int16_t p_id )
{
		if( p_id < 0 || p_id >= _load_wav_curr_index )
		{
				return 0;
		}
		
		_load_wav_start_addr = _load_wav_poll[p_id].data_buffer;
		_load_wav_curr_index = p_id;
		
		return 1;
}


wav_t * getWav( const int16_t p_id )
{
		if( p_id < 0 || p_id >= _load_wav_curr_index )
		{
				return NULL;
		}
		
		return &_load_wav_poll[p_id];
}
