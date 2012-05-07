////////////////////////////////////////////////////////////////////////////////////////
//
//
//		LibCircusCS : library for CIRCUS CS plug-in development 
//
//		rawVolumeIO.h: I/O function for raw volume file (.vol)  
//
//			main coder: Y. Nomura
//
//			update history
//
//			2012.05.02	start coding
//
//
////////////////////////////////////////////////////////////////////////////////////////

#define _CRT_SECURE_NO_DEPRECATE 

#include "../LibCircusCS.h"

#include "rawVolumeIO.private.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T> T*  
CircusCS_LoadRawVolumeFile(char* fileName, int size)
{
	T*    volume = NULL;

	FILE* fp = fopen(fileName, "rb");
	
	if(fp == NULL)
	{
		fprintf(stderr, "Fail to open the file (%s)\n", fileName);
	}
	else
	{
		fread((char *)volume, sizeof(T), size, fp);
		fclose(fp);
	}

	return volume;
}

