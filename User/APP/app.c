/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                             (c) Copyright 2013; Micrium, Inc.; Weston, FL
*
*                   All rights reserved.  Protected by international copyright laws.
*                   Knowledge of the source code may not be used to write a similar
*                   product.  This file may only be used in accordance with a license
*                   and should not be redistributed in any way.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                            EXAMPLE CODE
*
*                                       IAR Development Kits
*                                              on the
*
*                                    STM32F429II-SK KICKSTART KIT
*
* Filename      : app.c
* Version       : V1.00
* Programmer(s) : YS
*                 DC
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <includes.h>

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                 TCB
*********************************************************************************************************
*/

static  OS_TCB   AppTaskStartTCB;

static  OS_TCB   AppTaskShellTCB;
static  OS_TCB   AppTaskInputTCB;
static  OS_TCB   AppTaskADCTCB;


/*
*********************************************************************************************************
*                                                STACKS
*********************************************************************************************************
*/

static  CPU_STK  AppTaskStartStk[APP_TASK_START_STK_SIZE];

static  CPU_STK  AppTaskShellStk [ APP_TASK_Shell_STK_SIZE ];
static  CPU_STK  AppTaskInputStk [ APP_TASK_Input_STK_SIZE ];
static  CPU_STK  AppTaskADCStk 		[ APP_TASK_ADC_STK_SIZE ];


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  AppTaskStart  (void *p_arg);

static  void  AppTaskShell  ( void * p_arg );
static  void  AppTaskInput  ( void * p_arg );
static  void  AppTaskADC  ( void * p_arg );


/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/

int  main (void)
{
    OS_ERR  err;


    OSInit(&err);                                               /* Init uC/OS-III.                                      */

    OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,                /* Create the start task                                */
                 (CPU_CHAR   *)"App Task Start",
                 (OS_TASK_PTR ) AppTaskStart,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_START_PRIO,
                 (CPU_STK    *)&AppTaskStartStk[0],
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

    OSStart(&err);                                              /* Start multitasking (i.e. give control to uC/OS-III). */
		
		
}


/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  AppTaskStart (void *p_arg)
{
    CPU_INT32U  cpu_clk_freq;
    CPU_INT32U  cnts;
    OS_ERR      err;


   (void)p_arg;

    CPU_Init();
    BSP_Init();                                                 /* Initialize BSP functions                             */

    cpu_clk_freq = BSP_CPU_ClkFreq();                           /* Determine SysTick reference freq.                    */
    cnts = cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;        /* Determine nbr SysTick increments                     */
    OS_CPU_SysTickInit(cnts);                                   /* Init uC/OS periodic time src (SysTick).              */

    Mem_Init();                                                 /* Initialize Memory Management Module                  */

#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                               /* Compute CPU capacity with no task running            */
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif


    OSTaskCreate((OS_TCB     *)&AppTaskShellTCB,                /* Create the Led1 task                                */
                 (CPU_CHAR   *)"App Task Shell",
                 (OS_TASK_PTR ) AppTaskShell,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_Shell_PRIO,
                 (CPU_STK    *)&AppTaskShellStk[0],
                 (CPU_STK_SIZE) APP_TASK_Shell_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_Shell_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
								 
    OSTaskCreate((OS_TCB     *)&AppTaskInputTCB,                /* Create the Led2 task                                */
                 (CPU_CHAR   *)"App Task Input",
                 (OS_TASK_PTR ) AppTaskInput,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_Input_PRIO,
                 (CPU_STK    *)&AppTaskInputStk[0],
                 (CPU_STK_SIZE) APP_TASK_Input_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_Input_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

    OSTaskCreate((OS_TCB     *)&AppTaskADCTCB,                /* Create the Led3 task                                */
                 (CPU_CHAR   *)"App Task ADC",
                 (OS_TASK_PTR ) AppTaskADC,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_ADC_PRIO,
                 (CPU_STK    *)&AppTaskADCStk[0],
                 (CPU_STK_SIZE) APP_TASK_ADC_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_ADC_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
		
		
		OSTaskDel ( & AppTaskStartTCB, & err );
		
		
}


/*
*********************************************************************************************************
*                                        SHELL CMD TASK
*********************************************************************************************************
*/

static  void  AppTaskShell ( void * p_arg )
{
   (void)p_arg;

    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
			ShellLoop();
    }
		
		
}


/*
*********************************************************************************************************
*                                          LED2 TASK
*********************************************************************************************************
*/

static uint8_t Button_Order_Table[] = {
//		0,  1,	 2,  3, 	4, 	5, 	6, 	7, 	8, 	9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20
//		0,  4,   2, 19, 	17, 11, 15, 6, 10, 14, 12, 20, 	1, 	9, 	7, 	3, 	5, 13, 	8, 16, 18
//	
		0, 12, 	2,  15,  1, 	16,	7,	14,	18,	13, 8,	5,	10,	17,	9,	6,	19,	4,	20,	3,	11
};

static  void  AppTaskInput ( void * p_arg )
{
    OS_ERR      err;
   (void)p_arg;
	
		uint8_t t_input_states[21] = {0};
		uint8_t * t_read_input_states;
		uint8_t i, tmp;
		
		uint8_t t_on_button_list[21] = {0};
		uint8_t t_off_button_list[21] = {0};
		
		uint8_t t_on_button_list_length = 0;
		uint8_t t_off_button_list_length = 0;
		
		mode_djembe_init();
		
    while (DEF_TRUE) 
		{
				read_all_input_state( &t_read_input_states );
				for( i = 0; i < 21; ++i )
				{
						tmp = t_read_input_states[i];
						if( tmp != t_input_states[i] )
						{
								if( tmp )
								{
										t_on_button_list[ t_on_button_list_length++ ] = Button_Order_Table[ i ];
								}else{
										t_off_button_list[ t_off_button_list_length++ ] = Button_Order_Table[ i ];
								}
								t_input_states[i] = tmp;
						}
				}
				
				if( mode_app_tick != NULL )
				{
						mode_app_tick( t_on_button_list, t_on_button_list_length, t_off_button_list, t_off_button_list_length );
				}
				
				t_on_button_list_length = 0;
				t_off_button_list_length = 0;
			
				OSTimeDly ( 1, OS_OPT_TIME_DLY, & err );
    }
		
		
}


/*
*********************************************************************************************************
*                                          ADC TASK
*********************************************************************************************************
*/

static  void  AppTaskADC ( void * p_arg )
{
    OS_ERR      err;


   (void)p_arg;
		uint8_t i;

    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
				
				PCS_Data_Offset = ( PCS_Data_Offset + 1 ) % PCS_DATA_SIZE;
				
				for( i = 0; i < 4; ++i )
				{
						PCS_Data[i][ PCS_Data_Offset ] = ADC_ConvertedValue[i];
				}
			
				OSTimeDly ( 1, OS_OPT_TIME_DLY, & err );
    }
		
		
}






