////////////////////////////////////////////////////////////////////////////////////////
//
//
//		LibCircusCS : library for CIRCUS CS plug-in development 
//
//			winUtility.cpp : utility for windows 
//
//			main coder: Y. Nomura
//
//			update history
//
//			2011.03.23	start coding
//
//
////////////////////////////////////////////////////////////////////////////////////////

#define _CRT_SECURE_NO_DEPRECATE 

#include <windows.h>
#include <tchar.h>
#include <time.h>

#include "../LibCircusCS.h"

#include "winUtility.private.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int CircusCS_GetCoreNumber()
{
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	return sysinfo.dwNumberOfProcessors;
}

void CircusCS_DebugPrint(LPTSTR szFormat, ... )
{
	TCHAR   szBuffer[MAX_STRING_LENGTH + 1];
	INT     nWritten;
	va_list args;

	::ZeroMemory(szBuffer, sizeof(szBuffer));

	// Format error message like printf()
	va_start( args, szFormat );
	nWritten = _vsntprintf( szBuffer, MAX_STRING_LENGTH, szFormat, args );
	va_end( args );

	// Output debug string
	::OutputDebugString( szBuffer );

	return;
}

int CircusCS_AppendLogFile(char* fileName, char* message)
{
	char dateTimeStr[64];
	GetDateTimeString(dateTimeStr);

	FILE* fp = fopen(fileName, "a");

	if(fp==NULL)  return -1;

	fprintf(fp, "[%s] %s\n", dateTimeStr, message);

	fclose(fp);

	return 0;
}



void GetDateTimeString(char* dateTimeStr)
{
	// Get current date/time
	time_t     current;
	struct tm  *local;

	time(&current);					// Retrieve current date/time
	local = localtime(&current);    // Retrieve local time

	SYSTEMTIME	 now;
	GetLocalTime(&now);

	sprintf(dateTimeStr, "%04d-%02d-%02d %02d:%02d:%02d",
		1900 + local->tm_year,
		local->tm_mon + 1,
		local->tm_mday,
		local->tm_hour,
		local->tm_min,
		local->tm_sec);
	return;
}
