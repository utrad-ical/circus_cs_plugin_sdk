////////////////////////////////////////////////////////////////////////////////////////
//
//
//		LibCircusCS : library for CIRCUS CS plug-in development 
//
//			winUtility.h : utility for windows 
//
//			main coder: Y. Nomura
//
//			update history
//
//			2011.03.23	start coding
//
//
////////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>

#ifndef WIN_UTILITY_H

int  CircusCS_GetCoreNumber();

void CircusCS_DebugPrint(LPTSTR szFormat, ... );
int  CircusCS_AppendLogFile(char* fileName, char* message);

#endif

#define WIN_UTILITY_H
