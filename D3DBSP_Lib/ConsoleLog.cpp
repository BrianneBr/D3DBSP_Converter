#include "ConsoleLog.h"
#include <stdarg.h>

WORD defaultConsoleAttributes = NULL;
HANDLE hConsole = NULL;
FILE* hLogfile = nullptr;

const char* FILEMODE_STRINGS[] = {"r","w","a","r+","w+","a+"};

DWORD dwErrorCount = 0;
DWORD dwWarningCount = 0;

WORD GetConsoleTextAttribute (HANDLE hCon)
{
  CONSOLE_SCREEN_BUFFER_INFO con_info;
  GetConsoleScreenBufferInfo(hCon, &con_info);
  return con_info.wAttributes;
}


int D3DBSP_LIB_API Con_Init()
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	defaultConsoleAttributes = GetConsoleTextAttribute(hConsole);

	hLogfile = nullptr;

	return 0;
}

int D3DBSP_LIB_API Con_Init(const char* logfilePath, LOGFILE_MODE mode)
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	defaultConsoleAttributes = GetConsoleTextAttribute(hConsole);
	return Log_Init(logfilePath, mode);
}

int D3DBSP_LIB_API Log_Init(const char* logfilePath, LOGFILE_MODE mode)
{
	int result = fopen_s(&hLogfile,logfilePath,FILEMODE_STRINGS[mode]);
	Log_Printf("Logfile Initialized!\n");
	return result;
}

int D3DBSP_LIB_API Con_Printf(const char* fmt, ...)
{
	SetConsoleTextAttribute(hConsole, defaultConsoleAttributes);
	va_list ap;
	va_start(ap, fmt);
	int out = 1;
	if(hConsole)
		out = vprintf(fmt, ap);	
	if(hLogfile)
		vfprintf(hLogfile,fmt,ap);
	va_end(ap);
	return out;
}

int D3DBSP_LIB_API Con_Warning(const char* fmt, ...)
{
	SetConsoleTextAttribute(hConsole, 0xE);
	va_list ap;
	va_start(ap, fmt);
	int out = 1;
	if(hConsole)
		out = vprintf(fmt, ap);	
	if(hLogfile)
		vfprintf(hLogfile,fmt,ap);
	va_end(ap);
	SetConsoleTextAttribute(hConsole, defaultConsoleAttributes);
	dwWarningCount++;
	return out;
}

int D3DBSP_LIB_API Con_Error(const char* fmt, ...)
{
	SetConsoleTextAttribute(hConsole, 0xC);
	va_list ap;
	va_start(ap, fmt);
	int out = 1;
	if(hConsole)
		out = vprintf(fmt, ap);	
	if(hLogfile)
		vfprintf(hLogfile,fmt,ap);	
	va_end(ap);
	SetConsoleTextAttribute(hConsole, defaultConsoleAttributes);
	dwErrorCount++;
	return out;
}

int D3DBSP_LIB_API Log_Printf(const char* fmt, ...)
{
	int out = 1;
	va_list ap;
	va_start(ap, fmt);
	if(hLogfile)
		out = vfprintf(hLogfile,fmt,ap);	
	va_end(ap);
	return out;
}

DWORD D3DBSP_LIB_API Con_ErrorCount(void)
{
	return dwErrorCount;
}

DWORD D3DBSP_LIB_API Con_WarningCount(void)
{
	return dwWarningCount;
}

int D3DBSP_LIB_API Con_Restore(void)
{
	if(hLogfile)
		fclose(hLogfile);
	return SetConsoleTextAttribute(hConsole, defaultConsoleAttributes);
}