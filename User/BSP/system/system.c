
#include <includes.h>


#define SYSTEM_VERSION "dadpat   v1.0.0"

//**********************************************//
//							system version	  							//
//**********************************************//
const char * ShellCmdVersion = "version";

void ShellCmdCallFuncVersion( const uint8_t argc, const char ** argv )
{
		printf( "\n" );
		printf( "***********************************\r\n" );
		printf( "\n" );
		printf( "**         %s       **\r\n", SYSTEM_VERSION );
		printf( "\n" );
		printf( "***********************************\r\n" );
		
		printf( "\r\n\r\n" );
}

//**********************************************//
//									file cmd										//
//**********************************************//

static void BL8782_PDN_INIT(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd ( RCC_AHB1Periph_GPIOG, ENABLE); 							   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
  GPIO_Init(GPIOG, &GPIO_InitStructure);	
  
  GPIO_ResetBits(GPIOG,GPIO_Pin_9);
}


FATFS system_fs;	
DIR		system_dir;

char system_curr_path[SYSTEM_PATH_LEN_MAX];

extern char * CmdBuffer;
extern int32_t CmdRecvIndex;

uint8_t IsDirectoryPath( const char * p_path )
{
		FRESULT result;
	
		result = f_opendir( &system_dir, p_path );
		if( result != FR_OK )
		{
				return 0;
		}
		
		f_closedir( &system_dir );
		
		return 1;
}

uint8_t IsFilePath( const char * p_path )
{
		FRESULT result;
		FIL t_fil;
	
		result = f_open( &t_fil, p_path, FA_READ );
		
		if( result != FR_OK )
		{
				return 0;
		}
		
		f_close( &t_fil );
		
		return 1;
}
uint8_t pathMerge( char * p_master, const char * slave )
{
	
		uint16_t t_masterLen;
		t_masterLen = strlen( p_master );
	
	
		if( slave[0] != '/' && t_masterLen > 1 )
		{
				p_master[t_masterLen++] = '/';
		}
		
		sprintf( p_master + t_masterLen, "%s", slave );
	
		return 1;
}
uint8_t getFileName( const char * p_path, char * p_fileName )
{
		int32_t i;
		uint16_t t_pathLen;
	
		t_pathLen = strlen( p_path );
		
		for( i = t_pathLen - 1; i >= 0; --i )
		{
				if( p_path[i] == '/' )
				{
						break;
				}
		}
		
		strcpy( p_fileName, p_path + ( i < 0 ? 0 : i + 1 ) );
		
		return 1;
}
uint8_t parsePath( const char * p_in, char * p_out )
{
		char t_buffer[64][16];
		uint8_t t_dir_count = 0;
		uint16_t i, t_path_len, t_offset = 0, t_dir_len;
	
		if( !( p_in && p_out ) )
		{
				return 0;
		}
		
		if( p_in[0] == '/' )
		{
				strcpy( p_out, p_in );
		}else
		{
			sprintf( p_out , "%s%s%s", system_curr_path, ( strlen(system_curr_path) > 1 ? "/" : "" ), p_in );
		}
		
		t_path_len = strlen( p_out );
	
		for( i = 0; i < t_path_len; ++i )
		{
				if( p_out[i] == '/' || i == t_path_len - 1 )
				{
						t_dir_len = i - t_offset + ( i == t_path_len - 1 && p_out[i] != '/' ? 1 : 0 );
						memcpy( t_buffer[t_dir_count], p_out + t_offset, t_dir_len );
						t_buffer[t_dir_count][t_dir_len] = '\0';
						
						t_offset = i + 1;
					
						t_dir_len = strlen( t_buffer[t_dir_count] );
						if( t_dir_len == 0 )
						{
								continue;
						}
						
						if( t_dir_len == 1 && strcmp( t_buffer[t_dir_count], "." ) == 0 )
						{
								continue;
						}
						
						if( t_dir_len == 2 && strcmp( t_buffer[t_dir_count], ".." ) == 0 )
						{
								t_dir_count = t_dir_count > 0 ? t_dir_count - 1 : 0;
								continue;
						}
						
						t_dir_count++;
				}
		}
		
		t_offset = 0;
		p_out[t_offset++] = '/';
		for( i = 0; i < t_dir_count; ++i )
		{
				if( i )
				{
						p_out[t_offset++] = '/';
				}
				
				t_path_len = strlen( t_buffer[i] );
				memcpy( p_out + t_offset, t_buffer[i], t_path_len );
				t_offset += t_path_len;
		}
		p_out[t_offset] = '\0';
		
		return 1;
}

uint8_t f_delpath( const char * p_path )
{
		FRESULT result;
		DIR dir;
		FILINFO fno;
	
		char t_buffer[SYSTEM_PATH_LEN_MAX];
	
		strcpy( t_buffer, p_path );
	
		result = f_opendir( &dir, p_path );
	
		while( result == FR_OK && f_readdir( &dir, &fno ) )
		{
				if( strlen( fno.fname ) == 0 ) break;
				if( strcmp( fno.fname, "." ) == 0 ) continue;
				if( strcmp( fno.fname, ".." ) == 0 ) continue;
			
				pathMerge( t_buffer, fno.fname );
				
				if( IsDirectoryPath( t_buffer ) )
				{
						f_delpath( t_buffer );
				}
				result = f_unlink( t_buffer );
		}
		
		f_closedir( &dir );
		
		result = f_unlink( p_path );
		
		return result == FR_OK ? 1 : 0;
}
const char * ShellCmdLS = "ls";
void ShellCmdCallFuncLs( const uint8_t argc, const char ** argv )
{
		FRESULT result;
		FILINFO t_file;
		uint16_t i;
	
		char t_path[SYSTEM_PATH_LEN_MAX];
	
		if( argc >= 2 )
		{
				parsePath( argv[1], t_path );
		}else{
				strcpy( t_path, system_curr_path );
		}
		
		result = f_opendir( &system_dir, t_path );
		if( result != FR_OK )
		{
				printf( "ls: %s: No such file or directory \r\n", argv[1] );
				return;
		}
		
		for( i = 0; i < 1000; ++i );
	
		for( ;; )
		{
			result = f_readdir( &system_dir, &t_file );
		
			if( result != FR_OK )
			{
					printf( "read dir %s final \r\n", t_path );
			}
			
			if( strlen( t_file.fname ) <= 0 )
			{
					break;
			}
			
			printf( "%s \t ", t_file.fname );
		}
		
		printf( "\r\n" );
		
		f_closedir( &system_dir );
}

const char * ShellCmdPWD = "pwd";
void ShellCmdCallFuncPwd( const uint8_t argc, const char ** argv )
{
		printf( "%s\r\n", system_curr_path );
}

const char * ShellCmdCD = "cd";
void ShellCmdCallFuncCd( const uint8_t argc, const char ** argv )
{
		char t_path[1024];
	
		if( argc != 2 )
		{
				return;
		}
		
		parsePath( argv[1], t_path );
		
		if( !IsDirectoryPath( t_path ) )
		{
				printf( "cd: %s: No such directory \r\n", t_path );
				return;
		}
		
		strcpy( system_curr_path, t_path );
}
const char * ShellCmdCAT = "cat";
void ShellCmdCallFuncCat( const uint8_t argc, const char ** argv )
{
		FRESULT result;
		FIL t_fil;
		uint32_t t_read_size;
	
		char t_buffer[1024];
	
		if( argc < 2 )
		{
				return;
		}
		
		parsePath( argv[1], t_buffer );
				
		result = f_open( &t_fil, t_buffer, FA_READ );
		
		if( result != FR_OK )
		{
				printf( "cat: %s : No such file \r\n", argv[1] );
				return;
		}
		
		t_read_size = 0;
		do
		{
				f_read( &t_fil, t_buffer, sizeof( t_buffer ), &t_read_size );
				
				if( t_read_size > 0 )
				{
						t_buffer[t_read_size] = '\0';
						printf( "%s", t_buffer );
				}
		}while( t_read_size > 0 );
		
		printf( "\r\n" );
		
		f_close( &t_fil );
}

const char * ShellCmdCP = "cp";
void ShellCmdCallFuncCp( const uint8_t argc, const char ** argv )
{
		uint8_t t_isDir;
		uint16_t t_pathLen;
		int32_t i;
		uint32_t t_readSize, t_writeSize;
	
		FRESULT result;
		FIL t_sourceFil, t_targetFil;
		
		char t_readBuffer[1024];
		char t_sourcePath[SYSTEM_PATH_LEN_MAX];
		char t_targetFileName[32] = {0};
		char t_targetPath[SYSTEM_PATH_LEN_MAX];
	
		parsePath( argv[argc - 1], t_targetPath );
	
		if( argc < 3 )
		{
				printf( "cp source [..] target \r\n" );
				return;
		}
		
		t_isDir = IsDirectoryPath( t_targetPath );
		if( argc > 3 && !t_isDir )
		{
				printf( "cp: %s: No such directory \r\n", t_targetPath );
				return;
		}
		
		if( argc == 3 && !t_isDir )
		{
				t_pathLen = strlen( t_targetPath );
				
				for( i = t_pathLen - 1; i >= 0; --i )
				{
						if( t_targetPath[i] == '/' )
						{
								break;
						}
				}
				
				strcpy( t_targetFileName, t_targetPath + ( i < 0 ? 0 : i + 1 ) );
				
				t_targetPath[ i == 0 ? i + 1 : i ] = '\0';
				
				if( !IsDirectoryPath( t_targetPath ) )
				{
						printf( "cp: %s: No such directory \r\n", t_targetPath );
						return;
				}
		}
		
		for( i = 1; i < argc - 1; ++i )
		{
			
				parsePath( argv[i], t_sourcePath );
			
				result = f_open( &t_sourceFil, t_sourcePath, FA_READ );
				if( result != FR_OK )
				{
						printf( "cp: %s : No such file \r\n", argv[i] );
						continue;
				}
			
				if( i != 1 || !strlen( t_targetFileName ) )
				{
						getFileName( argv[i], t_targetFileName );
				}
				
				pathMerge( t_targetPath, t_targetFileName );
				
				result = f_open( &t_targetFil, t_targetPath, FA_CREATE_ALWAYS | FA_WRITE );
				if( result != FR_OK )
				{
						printf( "cp: %s : create file final \r\n", t_targetPath );
						f_close( &t_sourceFil );
						continue;
				}
				
				do
				{
						result = f_read( &t_sourceFil, t_readBuffer, sizeof( t_readBuffer ), &t_readSize );
						if( result != FR_OK )
						{
								printf( "cp: read final \r\n" );
								goto end;
						}
						if( t_readSize > 0 )
						{
								do
								{
										result = f_write( &t_targetFil, t_readBuffer, t_readSize, &t_writeSize );
										if( result != FR_OK )
										{
												printf( "cp: write final \r\n" );
												goto end;
										}
										
										if( t_writeSize == t_readSize )
										{
												break;
										}
								}while( t_writeSize > 0 );
						}
					
				}while( t_readSize > 0 );

end:
				f_close( &t_targetFil );
				f_close( &t_sourceFil );
		}
}
const char * ShellCmdRM = "rm";
void ShellCmdCallFuncRm( const uint8_t argc, const char ** argv )
{
		uint16_t i;
		char t_pathBuffer[SYSTEM_PATH_LEN_MAX];
		if( argc < 2 )
		{
				return;
		}
		
		for( i = 1; i < argc; ++i )
		{
				parsePath( argv[i], t_pathBuffer );
			
				f_delpath( t_pathBuffer );
		}
}
const char * ShellCmdMKDIR = "mkdir";
void ShellCmdCallFuncMkdir( const uint8_t argc, const char ** argv )
{
		uint16_t i;
		
		char t_buffer[SYSTEM_PATH_LEN_MAX];
		if( argc < 2 )
		{
				return;
		}
		
		for( i = 1; i < argc; ++i )
		{
				if( !parsePath( argv[i], t_buffer ) )
				{
						continue;
				}
				
				f_mkdir( t_buffer );
		}
}
const char * ShellCmdLoadWav = "loadwav";
wav_t * s_wav;
void ShellCmdCallFuncLoadWav( const uint8_t argc, const char ** argv )
{
		char t_buffer[SYSTEM_PATH_LEN_MAX];
//		OS_ERR  err;
	
		if( argc < 2 )
		{
				return;
		}
		if( !parsePath( argv[1], t_buffer ) )
		{
				return;
		}
		printf( "--->> id: %d \r\n", loadWav( &s_wav, t_buffer ) );
}

const char * ShellCmdPlayWav = "play";
void ShellCmdCallFuncPlayWav( const uint8_t argc, const char ** argv )
{
		if( argc < 2 )
		{
				return;
		}
		
		s_wav = getWav( atoi( argv[1] ) );
		play_wav( s_wav, PLAY_TYPE_BGM, 1 );
}

const char * ShellCmdPad = "padmode";
void ShellCmdCallFuncPadMode( const uint8_t argc, const char ** argv )
{
		OS_ERR  err;
	
		printf( "-----> load bgm.wav: %d \r\n", loadWav( NULL, "bgm.wav" ) );
		printf( "-----> load B.wav: %d \r\n", loadWav( NULL, "djembe/N_B.wav" ) );
		printf( "-----> load T.wav: %d \r\n", loadWav( NULL, "djembe/N_T.wav" ) );
		printf( "-----> load S.wav: %d \r\n", loadWav( NULL, "djembe/N_S.wav" ) );
	
		while( 1 )
		{
				if( CmdRecvIndex > 0 )
				{
						CmdRecvIndex = 0;
						
						if( CmdBuffer[0] == 'q' )
						{
								unloadWav( 0 );
								break;
						}
						
						s_wav = getWav( CmdBuffer[0] - '0' );
						
						if( !s_wav )
						{
								continue;
						}
						
						play_wav( s_wav, ( CmdBuffer[0] == '0' ? PLAY_TYPE_BGM : PLAY_TYPE_EFFECT ), 1 );
				}
				
				OSTimeDly ( 1, OS_OPT_TIME_DLY, & err );
		}
}
void SystemConfig( void )
{
	
	FRESULT result;
	
	BL8782_PDN_INIT();

	result = f_mount( &system_fs, "0:", 1);
	if(result!=FR_OK)
	{
		printf("mount tf card final\r\n");
		while(1);
	}
	
	result = f_opendir( &system_dir, "/" );
	if( result != FR_OK )
	{
			printf( "open dir / final \r\n" );
			while(1);
	}
	strcpy( system_curr_path, "/" );

	ShellConfig();
	
	ShellCmdRegex( ShellCmdVersion, ShellCmdCallFuncVersion );
	ShellCmdRegex( ShellCmdLS, ShellCmdCallFuncLs );
	ShellCmdRegex( ShellCmdCD, ShellCmdCallFuncCd );
	ShellCmdRegex( ShellCmdPWD, ShellCmdCallFuncPwd );
	ShellCmdRegex( ShellCmdCAT, ShellCmdCallFuncCat );
	ShellCmdRegex( ShellCmdCP, ShellCmdCallFuncCp );
	ShellCmdRegex( ShellCmdRM, ShellCmdCallFuncRm );
	ShellCmdRegex( ShellCmdMKDIR, ShellCmdCallFuncMkdir );
	ShellCmdRegex( ShellCmdLoadWav, ShellCmdCallFuncLoadWav );
	ShellCmdRegex( ShellCmdPlayWav, ShellCmdCallFuncPlayWav );
	ShellCmdRegex( ShellCmdPad, ShellCmdCallFuncPadMode );
	
}

