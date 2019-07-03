#ifndef __DJEMBE_MODE_H__
#define __DJEMBE_MODE_H__

#include <stdint.h>

void mode_djembe_init( void );

void mode_djembe_tick( const uint8_t * p_on_area_list, uint8_t p_on_area_count, const uint8_t * p_off_area_list, uint8_t p_off_area_count );

#endif //__DJEMBE_MODE_H__

