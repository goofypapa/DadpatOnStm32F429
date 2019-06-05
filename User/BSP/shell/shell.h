#ifndef __SHELL_H__
#define __SHELL_H__

#include "stdint.h"

void ShellConfig( void );

void ShellLoop( void );

void ShellCmdPut( const char * p_shellCmd );
void ShellCmdParse( void );

void ShellCmdRegex( const char * p_cmd, void(*p_callBack)( const uint8_t argc, const char ** argv ) );


#endif //__SHELL_H__
