#include <includes.h>

#define BEAT_LIFE_IDLE							0
#define BEAT_LIFE_GATHER_AREA				1
#define BEAT_LIFE_GATHER_POWER			2
#define BEAT_LIFE_COOL_DOWN					3


#define GATHER_AREA_TIME						3
#define GATHER_PAWER_TIME						2
#define COOL_DOWN_TIME							30
#define INTENSITY_THRESHOLD					20000 


static uint16_t tones[3];
uint8_t getTone( void );

void mode_djembe_init( void )
{
		mode_app_tick = NULL;
		unloadWav( 0 );
	
		tones[0] = loadWav( NULL, "djembe/N_B.wav" );
		tones[1] = loadWav( NULL, "djembe/N_T.wav" );
		tones[2] = loadWav( NULL, "djembe/N_S.wav" );
	
		mode_app_tick = mode_djembe_tick;
}

static uint16_t s_tick_time = 0;
static uint16_t s_beat_life_state = BEAT_LIFE_IDLE;
static uint8_t 	s_on_area_list[21] = {0};
static uint8_t	s_on_area_count = 0;
static uint16_t s_intensity = 0;
void mode_djembe_tick( const uint8_t * p_on_area_list, uint8_t p_on_area_count, const uint8_t * p_off_area_list, uint8_t p_off_area_count )
{
		uint8_t i, n;
		uint8_t exist;
	
		uint16_t t_adc_max_datas[4] = {0};
		
		//
		if( s_beat_life_state == BEAT_LIFE_IDLE )
		{
				s_tick_time = 1;
			
				if( p_on_area_count )
				{
					s_beat_life_state = BEAT_LIFE_GATHER_AREA;
					s_on_area_count = 0;
				}
		}
		
		//
		if( s_beat_life_state == BEAT_LIFE_GATHER_AREA )
		{
				for( i = 0; i < p_on_area_count; ++i )
				{
						exist = 0;
						for( n = 0; n < s_on_area_count; ++n )
						{
								if( s_on_area_list[n] == p_on_area_list[i] )
								{
										exist = 1;
										break;
								}
						}
						
						if( !exist )
						{
								s_on_area_list[ s_on_area_count++ ] = p_on_area_list[i];
						}
				}
				
				if( s_tick_time >= GATHER_AREA_TIME )
				{
					
						//printf( "---->> on area: " );
						for( i = 0; i < s_on_area_count; ++i )
						{
								LED_ON( s_on_area_list[i] );
								//printf( "%d ", s_on_area_list[i] );
								//play_wav( getWav( tones[ s_on_area_list[i] % 3 ] ), PLAY_TYPE_EFFECT, 1 );
						}
						
						//printf( "\r\n" );
						
						s_beat_life_state = BEAT_LIFE_GATHER_POWER;
						s_tick_time = 1;
				}
		}
		
		//
		if( s_beat_life_state == BEAT_LIFE_GATHER_POWER )
		{
				if( s_tick_time >= GATHER_PAWER_TIME )
				{
					
//						s_intensity = ( ADC_ConvertedValue[0] + ADC_ConvertedValue[1] + ADC_ConvertedValue[2] + ADC_ConvertedValue[3] ) / 4;
//						
//						if( s_intensity < INTENSITY_THRESHOLD )
//						{
//								s_beat_life_state = BEAT_LIFE_IDLE;
//								return;
//						}
						
						
						//printf( "----->> PCS_Data_Switch: %d, PCS_Data_Offset: %d, offset1: %d, offset2: %d \r\n", PCS_Data_Switch, PCS_Data_Offset, t_offset_1, t_offset_2 );

						for( i = 0; i < 4; ++i )
						{
							
								//printf( "------>> ADC: %d ", i );
								t_adc_max_datas[i] = 0;
								for( n = PCS_Data_Offset + 1; n < PCS_DATA_SIZE ; ++n )
								{
										//printf( "%d ", PCS_Data[i][ n ] );
										if( t_adc_max_datas[i] < PCS_Data[i][ n ] )
										{
												t_adc_max_datas[i] = PCS_Data[i][ n ];
										}
								}
								
								for( n = 0; n <= PCS_Data_Offset; ++n )
								{			
										//printf( "%d ", PCS_Data[i][ n ] );
										if( t_adc_max_datas[i] < PCS_Data[i][ n ] )
										{
												t_adc_max_datas[i] = PCS_Data[i][ n ];
										}
								}
								//printf( "\r\n" );
						}
						//printf( "------>> ADC: %d %d %d %d \r\n", t_adc_max_datas[0], t_adc_max_datas[1], t_adc_max_datas[2], t_adc_max_datas[3] );
						
						s_intensity = ( t_adc_max_datas[0] + t_adc_max_datas[1] + t_adc_max_datas[2] + t_adc_max_datas[3] ) / 4;
						//printf( "------>> intensity: %d \r\n", s_intensity );
						
						if( s_intensity < INTENSITY_THRESHOLD )
						{
								s_beat_life_state = BEAT_LIFE_IDLE;
								return;
						}
						
						//
						play_wav( getWav( tones[ getTone() ] ), PLAY_TYPE_EFFECT, 1 );
					
						s_beat_life_state = BEAT_LIFE_COOL_DOWN;
						s_tick_time = 1;
				}
		}
		
		//
		if( s_beat_life_state == BEAT_LIFE_COOL_DOWN  )
		{
				if( s_tick_time >= COOL_DOWN_TIME )
				{
						LED_OFF_All();
						s_beat_life_state = BEAT_LIFE_IDLE;
				}
		}
		
		s_tick_time++;
}


//static uint8_t s_bass_area[1] = { 0 };
static uint8_t s_bass_area_count = 0;
	
static uint8_t s_tone_area[8] = { 0 };
static uint8_t s_tone_area_count = 0;

//static uint8_t s_slap_area[12] = { 0 };
static uint8_t s_slap_area_count = 0;

uint8_t getTone( void )
{
		uint8_t i, n;
		uint8_t t_tone;
		uint8_t t_angle, t_tmp;
	
		s_bass_area_count = 0;
		s_tone_area_count = 0;
		s_slap_area_count = 0;
	
		for( i = 0; i < s_on_area_count; ++i )
		{
				if( s_on_area_list[i] <= 0 )
				{
						//s_bass_area[ s_bass_area_count++ ] = s_on_area_list[i];
						s_bass_area_count++;
						continue;
				}
				
				if( s_on_area_list[i] <= 8 )
				{
						s_tone_area[ s_tone_area_count++ ] = s_on_area_list[i];
						continue;
				}
				
				//s_slap_area[ s_slap_area_count++ ] = s_on_area_list[i];
				s_slap_area_count++;
		}
		
		do
		{
				if( s_bass_area_count )
				{
						if( s_tone_area_count >= 4 )
						{
								t_tone = 0;
								break;
						}
						
						t_angle = 0;
						for( i = 0; i < s_tone_area_count; ++i )
						{
								for( n = 0; n < s_tone_area_count; ++n )
								{
										if( n == i ) continue;
										
										t_tmp = abs( s_tone_area[i] - s_tone_area[n] );
										t_tmp = t_tmp > 4 ? 4 - t_tmp % 4 : t_tmp;
									
										if( t_angle < t_tmp )
										{
												t_angle = t_tmp;
										}
								}
						}
						
						if( t_angle >= 4 )
						{
								t_tone = 0;
								break;
						}
						
						if( s_tone_area_count )
						{
								t_tone = 1;
								break;
						}
						
						t_tone = 0;
						break;
				}
				
				if( s_tone_area_count >= s_slap_area_count )
				{
						t_tone = 1;
						break;
				}
				
				t_tone = 2;
		}while( 0 );
		
		//printf( "----->> tone: %d \r\n", t_tone );
	
		return t_tone;
}

