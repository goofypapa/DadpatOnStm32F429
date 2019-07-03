#ifndef __MODE_MANAGER_H__
#define __MODE_MANAGER_H__

#include <stdint.h>

typedef void( * f_mode_app_tick )( const uint8_t * p_on_area_list, uint8_t p_on_area_count, const uint8_t * p_off_area_list, uint8_t p_off_area_count );

extern f_mode_app_tick mode_app_tick;


#define PCS_DATA_SIZE						3

//
extern uint16_t PCS_Data[4][PCS_DATA_SIZE];
extern uint8_t  PCS_Data_Offset;

#endif //__MODE_MANAGER_H__

