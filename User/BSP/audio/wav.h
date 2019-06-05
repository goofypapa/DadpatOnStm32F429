#ifndef __WAV_H__
#define __WAV_H__

#include "stdint.h"

typedef struct _wav_riff_t{
    char id[5];               //ID:"RIFF"
    int32_t  size;           	//file_len - 8
    char type[5];             //type:"WAVE"
}wav_riff_t;


typedef struct _wav_format_t{
    char  id[5];              //ID:"fmt"
    int32_t   size;
    int16_t 	compression_code;
    int16_t 	channels;
    int32_t   samples_per_sec;
    int32_t   avg_bytes_per_sec;
    int16_t 	block_align;
    int16_t 	bits_per_sample;
}wav_format_t;


typedef struct _wav_fact_t{
    char 			id[5];
    int32_t  	size;
}wav_fact_t;


typedef struct _wav_data_t{
    char 			id[5];
    int32_t  	size;
}wav_data_t;


typedef struct _wav_t{
    wav_riff_t   riff;
    wav_format_t format;
    wav_fact_t   fact;
    wav_data_t   data;
    int32_t      file_size;
    int32_t      data_offset;
    char *       data_buffer;
}wav_t;


int16_t loadWav( wav_t ** p_wav, const char * p_filePath );
uint8_t unloadWav( const int16_t p_id );

wav_t * getWav( const int16_t p_id );

#endif //__WAV_H__
