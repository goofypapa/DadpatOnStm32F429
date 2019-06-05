/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ledӦ�ú����ӿ�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F429 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
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
  * @brief  ��ʼ������LED��IO
  * @param  ��
  * @retval ��
  */
void LED_GPIO_Config(void)
{		
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*����LED��ص�GPIO����ʱ��*/
		RCC_AHB1PeriphClockCmd ( LED1_GPIO_CLK|
	                           LED2_GPIO_CLK|
	                           LED3_GPIO_CLK, ENABLE); 

		/*ѡ��Ҫ���Ƶ�GPIO����*/															   
		GPIO_InitStructure.GPIO_Pin = LED1_PIN;	

		/*��������ģʽΪ���ģʽ*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
    
    /*�������ŵ��������Ϊ�������*/
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    
    /*��������Ϊ����ģʽ*/
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

		/*������������Ϊ2MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 

		/*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
		GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);	
    
    /*ѡ��Ҫ���Ƶ�GPIO����*/															   
		GPIO_InitStructure.GPIO_Pin = LED2_PIN;	
    GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);	
    
    /*ѡ��Ҫ���Ƶ�GPIO����*/															   
		GPIO_InitStructure.GPIO_Pin = LED3_PIN;	
    GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);	
		
		/*�ر�RGB��*/
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
