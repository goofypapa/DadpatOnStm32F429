#include "shell.h"
#include "stdio.h"
#include "string.h"
#include "stdint.h"
#include <includes.h>
#include "bsp_led.h"


#define SHELL_CMD_MAX 16
#define SHELL_CMD_MAX_LENGTH 128

#define SHELL_CMD_SOURCE_MAX 16

#define SHELL_CMD_CALL_FUNC_POOL_SIZE 1024

char * _shellCmdPoll[SHELL_CMD_MAX];
char shellCmdPoll[SHELL_CMD_MAX][SHELL_CMD_MAX_LENGTH];

char shellCmdSource[16][1024];

void * shellCmdCallFuncPool[SHELL_CMD_CALL_FUNC_POOL_SIZE][2];
uint16_t shellCmdCallFuncRegexIndex = 0;

uint16_t ShellCmdSourcePutIndex = 0;
uint16_t ShellCmdSourceParseIndex = 0;
uint16_t ShellCmdSourceLength = 0;
OS_MUTEX shellCmdSourceMutex;

void ShellConfig( void )
{
		OS_ERR  err;
		uint16_t i;
		
		for( i = 0; i < SHELL_CMD_MAX; ++i )
		{
				_shellCmdPoll[i] = shellCmdPoll[i];
		}
		
    OSMutexCreate ((OS_MUTEX  *)&shellCmdSourceMutex, 
                   (CPU_CHAR  *)"ShellCmdSourceMutex", 
                   (OS_ERR    *)&err);
}

void ShellLoop( void )
{
		OS_ERR  err;
	
		if( ShellCmdSourceLength )
		{
				while( ShellCmdSourceLength )
				{
						ShellCmdParse();
				}
				printf( "$ " );
		}
		else{
				OSTimeDly ( 1, OS_OPT_TIME_DLY, & err );
		}
		
}

void ShellCmdPut( const char * p_shellCmd )
{
		OS_ERR  err;
	
		OSMutexPend ((OS_MUTEX  *)&shellCmdSourceMutex, 
								 (OS_TICK    )0, 
								 (OS_OPT     )OS_OPT_PEND_BLOCKING,
								 (CPU_TS    *)0,
								 (OS_ERR    *)&err);
	
		if( ShellCmdSourceLength < SHELL_CMD_SOURCE_MAX )
		{

			strcpy( shellCmdSource[ShellCmdSourcePutIndex], p_shellCmd );
		
			ShellCmdSourcePutIndex = ShellCmdSourcePutIndex + 1 >= SHELL_CMD_SOURCE_MAX ? 0 : ShellCmdSourcePutIndex + 1;
		
			ShellCmdSourceLength++;
		}
	
		OSMutexPost ((OS_MUTEX  *)&shellCmdSourceMutex,
								 (OS_OPT     )OS_OPT_POST_NONE,
								 (OS_ERR    *)&err);	
}

void ShellCmdParse( void )
{
		OS_ERR  err;
		uint16_t i, t_offset, t_len;
		uint16_t t_parameterIndex = 0;
		char * t_cmd = shellCmdSource[ShellCmdSourceParseIndex];
		uint16_t t_cmd_len = strlen( t_cmd );
		char * t_str;
		
		for( i = 0, t_offset = 0; i < t_cmd_len; ++i )
		{
				if( t_cmd[i] == ' ' || t_cmd[i] == '\r' || t_cmd[i] == '\n' )
				{
						if( i - t_offset == 0 )
						{
								t_offset = i + 1;
								continue;
						}
						
						t_len = i - t_offset;
						if( t_len >= SHELL_CMD_MAX_LENGTH )
						{
								t_len = SHELL_CMD_MAX_LENGTH - 1;
						}
						
						memcpy( shellCmdPoll[t_parameterIndex], t_cmd + t_offset, t_len );
						shellCmdPoll[t_parameterIndex++][t_len] = '\0';
						t_offset = i + 1;
				}
		}
		
		OSMutexPend ((OS_MUTEX  *)&shellCmdSourceMutex, 
								 (OS_TICK    )0, 
								 (OS_OPT     )OS_OPT_PEND_BLOCKING,
								 (CPU_TS    *)0,
								 (OS_ERR    *)&err);
		
		ShellCmdSourceParseIndex = ShellCmdSourceParseIndex + 1 >= SHELL_CMD_SOURCE_MAX ? 0 : ShellCmdSourceParseIndex + 1;
		ShellCmdSourceLength--;
		
		OSMutexPost ((OS_MUTEX  *)&shellCmdSourceMutex,
								 (OS_OPT     )OS_OPT_POST_NONE,
								 (OS_ERR    *)&err);
		
		for( i = 0; i < shellCmdCallFuncRegexIndex; ++i )
		{
				t_str = (char *)shellCmdCallFuncPool[i][0];
				
				if( strlen( t_str ) != strlen( shellCmdPoll[0] ) || strcmp( t_str, shellCmdPoll[0] ) != 0 )
				{
						continue;
				}
				
				( ( void(*)( const uint8_t, const char ** ) )shellCmdCallFuncPool[i][1] )( t_parameterIndex, (const char **)_shellCmdPoll );
				break;
		}
		
		if( strlen( shellCmdPoll[0] ) > 0 && i == shellCmdCallFuncRegexIndex )
		{
				printf( "dadpat: %s: commond not found\r\n", shellCmdPoll[0] );
		}
		
		strcpy( shellCmdPoll[0], "" );
}

void ShellCmdRegex( const char * p_cmd, void(*p_callBack)( const uint8_t argc, const char ** argv ) )
{
		if( shellCmdCallFuncRegexIndex >= SHELL_CMD_CALL_FUNC_POOL_SIZE )
		{
				printf( "Shell Cmd Regex final! \n" );
				return;
		}
		
		shellCmdCallFuncPool[shellCmdCallFuncRegexIndex][0] = (void *)p_cmd;
		shellCmdCallFuncPool[shellCmdCallFuncRegexIndex][1] = (void *)p_callBack;
		shellCmdCallFuncRegexIndex++;
}
