#include <includes.h>


static uint16_t All_Led_Pins[] = {
		LED1_INT_GPIO_PIN,
		LED2_INT_GPIO_PIN,
		LED3_INT_GPIO_PIN,
		LED4_INT_GPIO_PIN,
		LED5_INT_GPIO_PIN,
		LED6_INT_GPIO_PIN,
		LED7_INT_GPIO_PIN,
		LED8_INT_GPIO_PIN,
		LED9_INT_GPIO_PIN,
		LED10_INT_GPIO_PIN,
		LED11_INT_GPIO_PIN,
		LED12_INT_GPIO_PIN,
		LED13_INT_GPIO_PIN,
		LED14_INT_GPIO_PIN,
		LED15_INT_GPIO_PIN,
		LED16_INT_GPIO_PIN,
		LED17_INT_GPIO_PIN,
		LED18_INT_GPIO_PIN,
		LED19_INT_GPIO_PIN,
		LED20_INT_GPIO_PIN,
		LED21_INT_GPIO_PIN
};

static GPIO_TypeDef * All_Led_Ports[] = {
		LED1_INT_GPIO_PORT,
		LED2_INT_GPIO_PORT,
		LED3_INT_GPIO_PORT,
		LED4_INT_GPIO_PORT,
		LED5_INT_GPIO_PORT,
		LED6_INT_GPIO_PORT,
		LED7_INT_GPIO_PORT,
		LED8_INT_GPIO_PORT,
		LED9_INT_GPIO_PORT,
		LED10_INT_GPIO_PORT,
		LED11_INT_GPIO_PORT,
		LED12_INT_GPIO_PORT,
		LED13_INT_GPIO_PORT,
		LED14_INT_GPIO_PORT,
		LED15_INT_GPIO_PORT,
		LED16_INT_GPIO_PORT,
		LED17_INT_GPIO_PORT,
		LED18_INT_GPIO_PORT,
		LED19_INT_GPIO_PORT,
		LED20_INT_GPIO_PORT,
		LED21_INT_GPIO_PORT
};


static uint8_t Led_Order_Table[] = {
//		0,  1,	 2,  3,  4, 	5, 	6, 	7, 	8, 	9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20
//		17, 16,  15, 14, 13, 12, 11, 10,  9, 20, 19, 18,  6,  5,  4,  3,  2,  1,  8,  7,  0
//	
		20, 17,  16, 15, 14, 13, 12,  19, 18, 8,  7,  6,  5,   4,  3,  2,  1,  0, 11, 10, 9
};


const char * ShellCmdLed = "led";
void ShellCmdCallFuncLed( const uint8_t argc, const char ** argv );

void LED_Init( void )
{
		uint8_t i;
		
		GPIO_InitTypeDef GPIO_InitStructure;
	
		RCC_AHB1PeriphClockCmd( LED1_INT_GPIO_CLK | LED11_INT_GPIO_CLK | LED17_INT_GPIO_CLK, ENABLE );
	
		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	
		for( i = 0; i < 21; ++i )
		{
				GPIO_InitStructure.GPIO_Pin = All_Led_Pins[i];
				GPIO_Init( All_Led_Ports[i], &GPIO_InitStructure );
		}
		
		ShellCmdRegex( ShellCmdLed, ShellCmdCallFuncLed );
}

void LED_ON( uint8_t p_id )
{
		uint8_t t_id;
		t_id = Led_Order_Table[p_id];
	
		GPIO_SetBits( All_Led_Ports[t_id], All_Led_Pins[t_id] );
}
void LED_OFF( uint8_t p_id )
{
		uint8_t t_id;
		t_id = Led_Order_Table[p_id];
	
		GPIO_ResetBits( All_Led_Ports[t_id], All_Led_Pins[t_id] );
}

void LED_ON_All( void )
{
		uint8_t i;
	
		for( i = 0; i < 21; ++i )
		{
				GPIO_SetBits( All_Led_Ports[i], All_Led_Pins[i] );
		}
}
void LED_OFF_All( void )
{
		uint8_t i;
	
		for( i = 0; i < 21; ++i )
		{
				GPIO_ResetBits( All_Led_Ports[i], All_Led_Pins[i] );
		}
}

void ShellCmdCallFuncLed( const uint8_t argc, const char ** argv )
{
		if( argc < 2 )
		{
				return;
		}
		
		if( strncmp( argv[1], "offall", 6 ) == 0 )
		{
				LED_OFF_All();
		}
		
		if( strncmp( argv[1], "onall", 5 ) == 0 )
		{
				LED_ON_All();
		}
}

