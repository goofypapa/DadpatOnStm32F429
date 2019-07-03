
#include <includes.h>

static uint16_t All_Input_Pins[] = {
		INPUT1_INT_GPIO_PIN,
		INPUT2_INT_GPIO_PIN,
		INPUT3_INT_GPIO_PIN,
		INPUT4_INT_GPIO_PIN,
		INPUT5_INT_GPIO_PIN,
		INPUT6_INT_GPIO_PIN,
		INPUT7_INT_GPIO_PIN,
		INPUT8_INT_GPIO_PIN,
		INPUT9_INT_GPIO_PIN,
		INPUT10_INT_GPIO_PIN,
		INPUT11_INT_GPIO_PIN,
		INPUT12_INT_GPIO_PIN,
		INPUT13_INT_GPIO_PIN,
		INPUT14_INT_GPIO_PIN,
		INPUT15_INT_GPIO_PIN,
		INPUT16_INT_GPIO_PIN,
		INPUT17_INT_GPIO_PIN,
		INPUT18_INT_GPIO_PIN,
		INPUT19_INT_GPIO_PIN,
		INPUT20_INT_GPIO_PIN,
		INPUT21_INT_GPIO_PIN
};
static GPIO_TypeDef * All_Input_Ports[] = {
		INPUT1_INT_GPIO_PORT,
		INPUT2_INT_GPIO_PORT,
		INPUT3_INT_GPIO_PORT,
		INPUT4_INT_GPIO_PORT,
		INPUT5_INT_GPIO_PORT,
		INPUT6_INT_GPIO_PORT,
		INPUT7_INT_GPIO_PORT,
		INPUT8_INT_GPIO_PORT,
		INPUT9_INT_GPIO_PORT,
		INPUT10_INT_GPIO_PORT,
		INPUT11_INT_GPIO_PORT,
		INPUT12_INT_GPIO_PORT,
		INPUT13_INT_GPIO_PORT,
		INPUT14_INT_GPIO_PORT,
		INPUT15_INT_GPIO_PORT,
		INPUT16_INT_GPIO_PORT,
		INPUT17_INT_GPIO_PORT,
		INPUT18_INT_GPIO_PORT,
		INPUT19_INT_GPIO_PORT,
		INPUT20_INT_GPIO_PORT,
		INPUT21_INT_GPIO_PORT
};

void Gating_Membrane_Init( void )
{
	
		uint8_t i;
		
		GPIO_InitTypeDef GPIO_InitStructure;
	
		RCC_AHB1PeriphClockCmd( INPUT1_INT_GPIO_CLK | INPUT12_INT_GPIO_CLK, ENABLE );
	
		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	
		for( i = 0; i < 21; ++i )
		{
				GPIO_InitStructure.GPIO_Pin = All_Input_Pins[i];
				GPIO_Init( All_Input_Ports[i], &GPIO_InitStructure );
		}
}


static uint8_t s_all_input_states[21] = {0};
void read_all_input_state( uint8_t ** p_out_states )
{
		uint8_t i;
		for( i = 0; i < 21; ++i )
		{
				s_all_input_states[i] = GPIO_ReadInputDataBit( All_Input_Ports[i], All_Input_Pins[i] );
		}
		
		*p_out_states = s_all_input_states;
}

