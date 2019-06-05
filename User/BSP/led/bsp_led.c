/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   led应用函数接口
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火  STM32 F429 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./led/bsp_led.h"
#include "shell.h"
#include "stdio.h"
#include "string.h"
#include <includes.h>


const char * ShellCmdLed = "led";

void ShellCmdRegexFuncLed( const uint8_t argc, const char ** argv );

 /**
  * @brief  初始化控制LED的IO
  * @param  无
  * @retval 无
  */
void LED_GPIO_Config(void)
{		
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启LED相关的GPIO外设时钟*/
		RCC_AHB1PeriphClockCmd ( LED1_GPIO_CLK|
	                           LED2_GPIO_CLK|
	                           LED3_GPIO_CLK, ENABLE); 

		/*选择要控制的GPIO引脚*/															   
		GPIO_InitStructure.GPIO_Pin = LED1_PIN;	

		/*设置引脚模式为输出模式*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
    
    /*设置引脚的输出类型为推挽输出*/
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    
    /*设置引脚为上拉模式*/
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

		/*设置引脚速率为2MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 

		/*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/
		GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);	
    
    /*选择要控制的GPIO引脚*/															   
		GPIO_InitStructure.GPIO_Pin = LED2_PIN;	
    GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);	
    
    /*选择要控制的GPIO引脚*/															   
		GPIO_InitStructure.GPIO_Pin = LED3_PIN;	
    GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);	
		
		/*关闭RGB灯*/
		LED_RGBOFF;		
		
		ShellCmdRegex( ShellCmdLed, ShellCmdRegexFuncLed );
}

void ShellCmdRegexFuncLed( const uint8_t argc, const char ** argv )
{
		uint8_t i;
		OS_ERR  err;
	
		if( argc != 2 )
		{
				return;
		}
		
		if( strcmp( argv[1], "red" ) == 0 )
		{
				LED_RED;
		}
		
		if( strcmp( argv[1], "green" ) == 0 )
		{
				LED_GREEN;
		}
		
		if( strcmp( argv[1], "blue" ) == 0 )
		{
				LED_BLUE;
		}
		
		if( strcmp( argv[1], "yellow" ) == 0 )
		{
				LED_YELLOW;
		}
		
		if( strcmp( argv[1], "purple" ) == 0 )
		{
				LED_PURPLE;
		}
		
		if( strcmp( argv[1], "cyan" ) == 0 )
		{
				LED_CYAN;
		}
		
		if( strcmp( argv[1], "white" ) == 0 )
		{
				LED_WHITE;
		}
		
		if( strcmp( argv[1], "off" ) == 0 )
		{
				LED_RGBOFF;
		}
		
		if( strcmp( argv[1], "twinkle" ) == 0 )
		{
				for( i = 0; i < 10; ++i )
				{
						LED1_TOGGLE;
						OSTimeDly ( 1000, OS_OPT_TIME_DLY, & err );
				}
		}
}

/*********************************************END OF FILE**********************/
